/*
 * A184400  Ranks of (odd i)+(even j)*r, when all i+j*r are ranked; r=sqrt(2),
 *          i>=0, j>=0.
 * https://oeis.org/A184400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   encode_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A184400: a(n) is the rank of the n-th pair with i odd and j even
   among all pairs (i,j) sorted by the value i + j*sqrt(2).  An implicit
   algorithmic search is used; the result is exact for any seq_t that fits
   the computed rank. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/encode_key.h"

/* ---------- helpers for integer key encoding ---------- */

/* ---------- simple open‑addressing hash table for uint64_t keys ---------- */
typedef struct {
    size_t size;      /* power of two table size */
    size_t count;     /* number of elements stored */
    uint64_t *table;  /* table of uint64_t keys; 0 is empty */
} U64Hash;

static size_t next_pow2(size_t v)
{
    size_t p = 1;
    while (p < v) p <<= 1;
    return p;
}

static void u64hash_init(U64Hash *h, size_t expected)
{
    h->size = next_pow2(expected * 2);   /* load factor < 0.5 */
    h->count = 0;
    h->table = (uint64_t *)calloc(h->size, sizeof(uint64_t));
}

static bool u64hash_contains(U64Hash *h, uint64_t key)
{
    size_t i = key & (h->size - 1);
    while (h->table[i]) {
        if (h->table[i] == key) return true;
        i = (i + 1) & (h->size - 1);
    }
    return false;
}

static void u64hash_insert(U64Hash *h, uint64_t key)
{
    size_t i = key & (h->size - 1);
    while (h->table[i]) {
        if (h->table[i] == key) return;
        i = (i + 1) & (h->size - 1);
    }
    h->table[i] = key;
    ++h->count;
}

/* ---------- heap of (i,j) pairs ---------- */
typedef struct {
    idx_t i;
    idx_t j;
} Node;

typedef struct {
    size_t size;
    size_t cap;
    Node *data;
} Heap;

static void heap_init(Heap *h)
{
    h->size = 0;
    h->cap  = 8;
    h->data = (Node *)calloc(h->cap, sizeof(Node));
}
static void heap_free(Heap *h)
{
    free(h->data);
}

/* comparator: true if a < b in the ordering induced by i + j*sqrt(2) */
static bool node_less(const Node *a, const Node *b)
{
    if (a->j == b->j) return a->i < b->i;

    /* Ensure the difference in j is positive */
    if (a->j > b->j) {
        int64_t di = (int64_t)a->i - (int64_t)b->i;
        int64_t dj = (int64_t)a->j - (int64_t)b->j;   /* positive */
        if (di >= 0) return false;                    /* cannot be less */
        /* need to check (-di)^2 > 2 * dj^2 */
        unsigned __int128 di2 = (unsigned __int128)(-di) * (unsigned __int128)(-di);
        unsigned __int128 dj2 = (unsigned __int128)dj * (unsigned __int128)dj;
        return di2 > (unsigned __int128)2 * dj2;
    } else {
        /* a->j < b->j : swap roles and invert result */
        return !node_less(b, a);
    }
}

/* swap two nodes */
static inline void heap_swap(Node *x, Node *y)
{
    Node t = *x; *x = *y; *y = t;
}

static void heap_push(Heap *h, Node v)
{
    if (h->size == h->cap) {
        h->cap <<= 1;
        h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
    }
    size_t idx = h->size++;
    h->data[idx] = v;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (node_less(&h->data[idx], &h->data[parent])) {
            heap_swap(&h->data[idx], &h->data[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

static Node heap_pop(Heap *h)
{
    Node top = h->data[0];
    h->size--;
    if (h->size) {
        h->data[0] = h->data[h->size];
        size_t idx = 0;
        while (true) {
            size_t left = (idx << 1) + 1;
            if (left >= h->size) break;
            size_t right = left + 1;
            size_t min_child = (right < h->size &&
                               node_less(&h->data[right], &h->data[left]))
                               ? right : left;
            if (node_less(&h->data[min_child], &h->data[idx])) {
                heap_swap(&h->data[idx], &h->data[min_child]);
                idx = min_child;
            } else {
                break;
            }
        }
    }
    return top;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n>=1, but safe fallback */

    /* Rough capacity estimate; each required pair may need up to ~4 extra nodes */
    size_t cap_est = n * 4 + 16;

    /* visited set to avoid duplicates */
    U64Hash visited;
    u64hash_init(&visited, cap_est);

    /* min‑heap of frontier pairs */
    Heap pq;
    heap_init(&pq);

    /* start with (0,0) */
    Node start = {0, 0};
    heap_push(&pq, start);
    u64hash_insert(&visited, encode_key(0, 0));

    idx_t rank = 0;   /* 1‑based rank among all pairs */
    idx_t found = 0;  /* number of (odd i, even j) pairs seen */

    while (true) {
        Node cur = heap_pop(&pq);
        rank++;   /* advance global rank */

        if ( ((cur.i & 1u) != 0) && ((cur.j & 1u) == 0u) ) {
            /* (i odd, j even) pair */
            found++;
            if (found == n) {
                /* clean up and return result */
                heap_free(&pq);
                free(visited.table);
                return (seq_t)rank;
            }
        }

        /* generate neighbors (i+1,j) and (i,j+1) */
        Node nb1 = { cur.i + 1, cur.j };
        uint64_t k1 = encode_key(nb1.i, nb1.j);
        if (!u64hash_contains(&visited, k1)) {
            u64hash_insert(&visited, k1);
            heap_push(&pq, nb1);
        }
        Node nb2 = { cur.i, cur.j + 1 };
        uint64_t k2 = encode_key(nb2.i, nb2.j);
        if (!u64hash_contains(&visited, k2)) {
            u64hash_insert(&visited, k2);
            heap_push(&pq, nb2);
        }
    }
}
