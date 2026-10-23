/*
 * A100272  Primes of the form a^5 + b^2 with a>0.
 * https://oeis.org/A100272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow5_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form a^5 + b^2 with a > 0 */
/* Search and merge method: for each a we walk the sequence of
   a^5 + b^2 (b = 0,1,2,…) using a min‑heap; each popped value
   is tested for primality.  Primes are stored in a cache in the
   order they first appear.  This allows cheap O(1) lookup for
   arbitrary n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow5_uint64.h"

/* ---------- helpers --------------------------------------------------- */

/* 64‑bit modular multiplication (using 128‑bit unsigned) */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* 64‑bit modular exponentiation */
static inline uint64_t pow_mod_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod_uint64(r, a, mod);
        a = mul_mod_uint64(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin bases for 64‑bit numbers */
static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- heap for merging ---------------------------------------- */
typedef struct {
    seq_t val;      /* current value a^5 + b^2 */
    uint64_t a;     /* fixed a for this sequence */
    uint64_t b;     /* current b */
} Node;

static Node *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz  = 0;

static inline void heap_swap(Node *x, Node *y)
{
    Node tmp = *x;
    *x = *y;
    *y = tmp;
}

static void heap_push(Node n)
{
    if (heap_sz == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 64;
        heap = (Node*)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_sz++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

static Node heap_pop(void)
{
    Node root = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return root;
}

/* ---------- prime cache ---------------------------------------------- */
static seq_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

/* ---------- global state for a's ------------------------------------- */
static uint64_t next_a_to_add = 1;   /* next a to be considered */

/* ---------- main function --------------------------------------------- */
seq_t a(idx_t n)
{
    /* lazy allocation of structures */
    if (!prime_cache) {
        prime_cap = 64;
        prime_cache = (seq_t*)malloc(prime_cap * sizeof(seq_t));
    }
    if (!heap) {
        heap_cap = 64;
        heap = (Node*)malloc(heap_cap * sizeof(Node));
    }

    /* generate primes until we have at least n */
    while (prime_len < (size_t)n) {
        /* if heap is empty, start with the first a */
        if (heap_sz == 0) {
            uint64_t a_val = next_a_to_add++;
            uint64_t a5    = pow5_uint64(a_val);
            heap_push((Node){(seq_t)a5, a_val, 0});
            continue; /* will pop this entry in next loop */
        }

        /* take the smallest candidate */
        Node cur = heap_pop();

        /* push the next value for the same a (b+1) */
        uint64_t b_next = cur.b + 1;
        seq_t next_val = cur.val + (seq_t)(2 * cur.b + 1);  /* a^5 + (b+1)^2 = a^5 + b^2 + (2b+1) */
        heap_push((Node){next_val, cur.a, b_next});

        /* add new a's whose starting value is <= current min of heap */
        while (heap_sz > 0) {
            uint64_t a_candidate   = next_a_to_add;
            uint64_t a5_candidate  = pow5_uint64(a_candidate);
            seq_t   min_val       = heap[0].val;
            if ((seq_t)a5_candidate <= min_val) {
                heap_push((Node){(seq_t)a5_candidate, a_candidate, 0});
                ++next_a_to_add;
            } else {
                break;
            }
        }

        /* check primality of the popped value */
        uint64_t val64 = (uint64_t)cur.val;  /* cur.val fits in 64 bits in our expected range */
        if (is_prime_uint64(val64)) {
            /* avoid duplicates */
            if (prime_len == 0 || (uint64_t)prime_cache[prime_len - 1] != val64) {
                if (prime_len == prime_cap) {
                    prime_cap *= 2;
                    prime_cache = (seq_t*)realloc(prime_cache, prime_cap * sizeof(seq_t));
                }
                prime_cache[prime_len++] = cur.val;
            }
        }
    }

    return prime_cache[(size_t)n - 1];
}
