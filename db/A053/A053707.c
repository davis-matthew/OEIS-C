/*
 * A053707  First differences of A025475, powers of a prime but not prime.
 * https://oeis.org/A053707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A025475(n+1) – A025475(n) (1‑based indices) A025475:
 *            powers of a prime but not prime (1 included)
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   build_primes, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"
#include "../utils/build_primes.h"

/* Node for the min‑heap – current prime power and its base prime */
typedef struct {
    seq_t val;      /* current power value */
    idx_t prime;    /* base prime */
} Node;

/* ---------- Simple binary min‑heap for Nodes ---------- */
static struct {
    Node *data;   /* heap array */
    size_t size;  /* used elements */
    size_t cap;   /* allocated capacity */
} heap = { NULL, 0, 0 };

static inline bool node_lt(const Node *a, const Node *b)
{
    return a->val < b->val;
}

static void heap_swap(size_t i, size_t j)
{
    Node tmp = heap.data[i];
    heap.data[i] = heap.data[j];
    heap.data[j] = tmp;
}

static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (!node_lt(&heap.data[i], &heap.data[parent]))
            break;
        heap_swap(i, parent);
        i = parent;
    }
}

static void heap_sift_down(size_t i)
{
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < heap.size && node_lt(&heap.data[left], &heap.data[smallest]))
            smallest = left;
        if (right < heap.size && node_lt(&heap.data[right], &heap.data[smallest]))
            smallest = right;

        if (smallest == i)
            break;
        heap_swap(i, smallest);
        i = smallest;
    }
}

static void heap_push(Node node)
{
    if (heap.size == heap.cap) {
        size_t new_cap = heap.cap ? (heap.cap << 1) : 16;
        Node *tmp = (Node *)realloc(heap.data, new_cap * sizeof(Node));
        if (!tmp) exit(EXIT_FAILURE);
        heap.data = tmp;
        heap.cap = new_cap;
    }
    heap.data[heap.size++] = node;
    heap_sift_up(heap.size - 1);
}

static Node heap_pop_min(void)
{
    Node min = heap.data[0];
    heap.data[0] = heap.data[--heap.size];
    heap_sift_down(0);
    return min;
}

/* ---------- Storage for A025475 values ---------- */
static struct {
    seq_t *data;   /* array of values, 1‑based (data[0] = 1) */
    size_t len;    /* number of stored values (including data[0]) */
    size_t cap;    /* allocated capacity */
} powers = { NULL, 0, 0 };

/* ---------- Storage for primes used in the heap ---------- */
static struct {
    idx_t *data;   /* list of primes */
    size_t len;    /* number of primes */
    size_t cap;    /* capacity (unused after init) */
} primes = { NULL, 0, 0 };

static bool initialized = false;

seq_t a(idx_t n)
{
    /* n >= 1 required by the sequence */
    if (!initialized) {
        /* --- First‑time initialisation ----------------------------------- */
        const idx_t PRIME_LIMIT = 200000;          /* enough for n ≤ 10000 */
        build_primes(PRIME_LIMIT, &primes.data, &primes.len);
        primes.cap = primes.len;                  /* fixed size */

        /* Allocate array for A025475 values */
        powers.cap = 128;
        powers.len = 1;                  /* powers[0] = 1 */
        powers.data = (seq_t *)malloc(powers.cap * sizeof(seq_t));
        if (!powers.data) exit(EXIT_FAILURE);
        powers.data[0] = (seq_t)1;

        /* Initialise the heap with p^2 for every prime */
        for (size_t i = 0; i < primes.len; ++i) {
            idx_t p = primes.data[i];
            seq_t val = (seq_t)p * (seq_t)p;
            heap_push((Node){ val, p });
        }
        initialized = true;
    }

    /* Generate A025475 values until we have at least n+1 of them  */
    while (powers.len <= (size_t)n) {         /* need indices 0…n inclusive */
        Node cur = heap_pop_min();            /* smallest un‑processed power */

        /* Append cur.val to powers array */
        if (powers.len == powers.cap) {
            powers.cap <<= 1;
            if (powers.cap == 0) powers.cap = 16;
            seq_t *tmp = (seq_t *)realloc(powers.data, powers.cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            powers.data = tmp;
        }
        powers.data[powers.len++] = cur.val;

        /* Push the next power for the same prime: cur.val * prime */
        seq_t next_val = mul_small(cur.val, (unsigned)cur.prime);
        heap_push((Node){ next_val, cur.prime });
    }

    /* Compute the difference: a(n) = A025475(n+1) – A025475(n) */
    return powers.data[n] - powers.data[n - 1];
}
