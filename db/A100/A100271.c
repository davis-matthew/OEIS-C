/*
 * A100271  Primes of the form a^4 + b^3 with b>0.
 * https://oeis.org/A100271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form a^4 + b^3 with b>0
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   a4_plus_b3, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/a4_plus_b3.h"

/* Node for the priority queue – smallest value first. */
typedef struct {
    unsigned __int128 val;  /* a^4 + b^3 */
    uint64_t          a;
    uint64_t          b;
} Node;

/* Heap parameters. */
#define INIT_CAP 1024
#define MAX_A    20000   /* ample enough for n up to about 10000 */

/* Static storage for the heap. */
static Node   *heap      = NULL;
static size_t heap_size  = 0;
static size_t heap_cap   = 0;

/* Static storage for found primes. */
static seq_t   *primes  = NULL;   /* array of primes */
static size_t   p_cnt   = 0;
static size_t   p_cap   = 0;

/* Next a to be inserted into the heap. */
static uint64_t next_a  = 2;       /* we seed with a=1 later */

/* Last prime value inserted (64‑bit) to suppress duplicates. */
static uint64_t last_prime_val = 0;

/* Compare nodes – smaller value has priority. */
static inline bool node_lt(const Node *x, const Node *y)
{
    if (x->val != y->val) return x->val < y->val;
    if (x->a   != y->a  ) return x->a   < y->a;
    return x->b < y->b;
}

/* Swap two nodes. */
static inline void swap(Node *x, Node *y)
{
    Node tmp = *x; *x = *y; *y = tmp;
}

/* Push a node onto the binary heap. */
static void heap_push(Node n)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : INIT_CAP;
        Node *tmp = (Node*)realloc(heap, new_cap * sizeof(Node));
        if (!tmp) exit(EXIT_FAILURE);
        heap = tmp; heap_cap = new_cap;
    }
    heap[heap_size++] = n;
    size_t i = heap_size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!node_lt(&heap[i], &heap[p])) break;
        swap(&heap[i], &heap[p]);
        i = p;
    }
}

/* Pop the minimum element from the heap. */
static Node heap_pop_min(void)
{
    Node ret = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t child = (r < heap_size && node_lt(&heap[r], &heap[l])) ? r : l;
        if (!node_lt(&heap[child], &heap[i])) break;
        swap(&heap[child], &heap[i]);
        i = child;
    }
    return ret;
}

/* Append a prime to the primes array. */
static void append_prime(seq_t v)
{
    if (p_cnt == p_cap) {
        size_t new_cap = p_cap ? p_cap * 2 : INIT_CAP;
        seq_t *tmp = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp; p_cap = new_cap;
    }
    primes[p_cnt++] = v;
}

/* The required function. */
seq_t a(idx_t n)
{
    /* Initialise heap once. */
    if (!heap) {
        heap = (Node*)malloc(INIT_CAP * sizeof(Node));
        if (!heap) exit(EXIT_FAILURE);
        heap_cap = INIT_CAP;
    }

    /* Initialise primes array once. */
    if (!primes) {
        primes = (seq_t*)malloc(INIT_CAP * sizeof(seq_t));
        if (!primes) exit(EXIT_FAILURE);
        p_cap = INIT_CAP;
    }

    /* Seed heap with the first candidate (a=1,b=1). */
    if (heap_size == 0) {
        heap_push((Node){a4_plus_b3(1, 1), 1, 1});
    }

    /* Generate primes until we have at least n of them. */
    while (p_cnt < (size_t)n) {
        Node cur = heap_pop_min();

        /* Only test values that fit in 64 bits for primality. */
        if ((cur.val >> 64) == 0) {
            uint64_t val64 = (uint64_t)cur.val;
            if (is_prime_uint64(val64) && val64 != last_prime_val) {
                append_prime((seq_t)val64);
                last_prime_val = val64;
            }
        }

        /* Push the next b for the same a. */
        heap_push((Node){a4_plus_b3(cur.a, cur.b + 1), cur.a, cur.b + 1});

        /* Insert a new a with b=1, up to MAX_A. */
        if (next_a <= MAX_A) {
            heap_push((Node){a4_plus_b3(next_a, 1), next_a, 1});
            ++next_a;
        }
    }

    return primes[n - 1];
}
