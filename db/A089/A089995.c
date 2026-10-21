/*
 * A089995  Products of pairs of distinct, non-consecutive primes.
 * https://oeis.org/A089995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089995: Products of pairs of distinct, non‑consecutive primes
   (sorted increasingly, 1‑based indexing).  The sequence consists of
   all numbers of the form p_i * p_j, where p_i and p_j are distinct
   primes, i < j and j > i + 1. */

/* ----- required headers ------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- limits ----------------------------------------------------------- */
#define MAX_PRIMES     25000   /* plenty for primes up to ≈300 000      */
#define MAX_SEQ        20000   /* store more than 10 000 terms           */

/* ----- global storage --------------------------------------------------- */
static uint64_t prime_arr[MAX_PRIMES];
static size_t   prime_cnt = 0;

static seq_t   seq_arr[MAX_SEQ];
static size_t  seq_len = 0;

/* ----- minimal binary heap for (product, i, j) ------------------------ */
typedef struct {
    seq_t prod;          /* product p_i * p_j                      */
    size_t i, j;         /* indices of the two primes              */
} MinNode;

static MinNode heap_arr[MAX_PRIMES];
static size_t heap_len = 0;

static void heap_swap(size_t a, size_t b)
{
    MinNode tmp = heap_arr[a];
    heap_arr[a] = heap_arr[b];
    heap_arr[b] = tmp;
}

static void heap_push(MinNode node)
{
    size_t idx = heap_len++;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap_arr[parent].prod <= node.prod)
            break;
        heap_arr[idx] = heap_arr[parent];
        idx = parent;
    }
    heap_arr[idx] = node;
}

static MinNode heap_popmin(void)
{
    MinNode top = heap_arr[0];
    size_t idx = 0;
    MinNode last = heap_arr[--heap_len];
    while (true) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        if (left >= heap_len)
            break;
        size_t child = left;
        if (right < heap_len && heap_arr[right].prod < heap_arr[left].prod)
            child = right;
        if (heap_arr[child].prod >= last.prod)
            break;
        heap_arr[idx] = heap_arr[child];
        idx = child;
    }
    heap_arr[idx] = last;
    return top;
}

/* ----- prime sieve ----------------------------------------------------- */
static void generate_primes(size_t limit)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) exit(1);

    for (uint32_t p = 2; p <= limit && prime_cnt < MAX_PRIMES; ++p) {
        if (!is_comp[p]) {
            prime_arr[prime_cnt++] = p;
            if ((uint64_t)p * p <= limit) {
                for (uint32_t q = p * p; q <= limit; q += p)
                    is_comp[q] = true;
            }
        }
    }
    free(is_comp);
}

/* ----- build enough terms --------------------------------------------- */
static void build_sequence(size_t need)
{
    /* Ensure we have a sufficient list of primes */
    generate_primes(300000);          /* generous upper bound          */

    /* Reset the heap and sequence buffer */
    heap_len = 0;
    seq_len  = 0;

    /* Initialise heap: for each prime p_i, pair with p_{i+2} */
    for (size_t i = 0; i + 2 < prime_cnt; ++i) {
        size_t j = i + 2;
        seq_t prod = (seq_t)prime_arr[i] * (seq_t)prime_arr[j];
        heap_push((MinNode){ prod, i, j });
    }

    /* Extract the smallest products until we have enough */
    while (seq_len < need && heap_len > 0) {
        MinNode cur = heap_popmin();
        seq_arr[seq_len++] = cur.prod;

        /* Prepare the next product with the same first prime */
        size_t i = cur.i;
        size_t j = cur.j + 1;
        if (j < prime_cnt) {
            seq_t prod = (seq_t)prime_arr[i] * (seq_t)prime_arr[j];
            heap_push((MinNode){ prod, i, j });
        }
    }
}

/* ----- public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined below offset, harmless */

    /* Build the sequence up to the requested index */
    if (seq_len < (size_t)n)
        build_sequence((size_t)n);

    return seq_arr[n - 1];       /* a(1) is the first term */
}
