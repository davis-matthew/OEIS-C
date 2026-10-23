/*
 * A180328  Semiprimes pq such that there is another semiprime rs with
 *          (p+1)(q+1)=(r+1)(s+1) and p, q, r, and s distinct primes.
 * https://oeis.org/A180328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th semiprime p*q such that for some other distinct
 *            primes r,s we have (p+1)(q+1) = (r+1)(s+1). The sequence is
 *            obtained by collecting all semiprimes up to a sufficiently large
 *            limit, computing the sigma value (p+1)(q+1) for each pair,
 *            keeping only those sigma values that occur at le...
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array()  */
#include "../utils/seq_realloc.h"      /* seq_realloc()      */

typedef struct {
    seq_t    pq;       /* product p*q   */
    uint64_t sigma;    /* (p+1)(q+1)    */
} spair_t;

/* ------------------------------------------------------------------ */
/* Comparator helpers (C++ requires explicit casts from void*)       */
static int cmp_spair(const void *a, const void *b)
{
    const spair_t *x = (const spair_t *)a;
    const spair_t *y = (const spair_t *)b;
    if (x->sigma < y->sigma) return -1;
    if (x->sigma > y->sigma) return 1;
    return 0;
}

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Dynamic array for the final sequence                                */
static seq_t *seq = NULL;   /* data buffer                         */
static size_t seq_cap = 0;  /* current allocated capacity          */
static size_t seq_len = 0;  /* number of filled elements           */

/* Ensure room for at least new_cap elements */
static void grow_seq(size_t new_cap)
{
    if (seq_cap >= new_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 64;
    if (cap < new_cap) cap = new_cap;
    seq = seq_realloc(seq, seq_cap, cap);  /* realloc for seq_t */
    seq_cap = cap;
}

static void append_seq(seq_t v)
{
    grow_seq(seq_len + 1);
    seq[seq_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Prime sieve – static because it is reused for all calls            */
static char *prime_flags = NULL;
static size_t prime_cnt = 0;
static uint32_t *prime_list = NULL;
#define LIMIT 5000000UL    /* enough to produce >10 000 terms */

static void build_primes(void)
{
    if (prime_flags != NULL) return;  /* already built */

    prime_flags = (char *)malloc((LIMIT + 1) * sizeof(char));
    for (size_t i = 0; i <= LIMIT; ++i) prime_flags[i] = 1;
    prime_flags[0] = prime_flags[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                prime_flags[j] = 0;
        }
    }

    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (prime_flags[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    for (size_t i = 2, k = 0; i <= LIMIT; ++i)
        if (prime_flags[i]) prime_list[k++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/* Generate the whole sequence once ---------------------------------- */
static void generate_sequence(void)
{
    if (seq_len > 0) return;   /* already computed */

    build_primes();

    /* 1. Build list of all semiprime pairs p<q with p*q <= LIMIT */
    size_t pairs_cap = 0, pairs_len = 0;
    spair_t *pairs = NULL;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t q = prime_list[j];
            uint64_t prod = p * q;
            if (prod > LIMIT) break;
            if (pairs_len == pairs_cap) {
                pairs_cap = pairs_cap ? pairs_cap << 1 : 64;
                pairs = (spair_t *)realloc(pairs, pairs_cap * sizeof(spair_t));
            }
            pairs[pairs_len].pq = (seq_t)prod;
            pairs[pairs_len].sigma = (p + 1) * (q + 1);
            ++pairs_len;
        }
    }

    /* 2. Sort pairs by sigma */
    qsort(pairs, pairs_len, sizeof(spair_t), cmp_spair);

    /* 3. Keep groups with at least two equal sigma values */
    size_t idx = 0;
    while (idx < pairs_len) {
        size_t start = idx;
        uint64_t cur_sigma = pairs[idx].sigma;
        ++idx;
        while (idx < pairs_len && pairs[idx].sigma == cur_sigma) ++idx;
        size_t group = idx - start;
        if (group >= 2) {
            for (size_t k = start; k < idx; ++k)
                append_seq(pairs[k].pq);
        }
    }

    free(pairs);
    free(prime_flags);
    free(prime_list);
    prime_flags = NULL;
    prime_list = NULL;
    prime_cnt = 0;

    /* 4. Sort final sequence ascending */
    qsort(seq, seq_len, sizeof(seq_t), cmp_seq);
}

/* ------------------------------------------------------------------ */
/* Public API – required signature ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* defined for n>=1 */
        return (seq_t)0;

    if (seq_len < (size_t)n)
        generate_sequence();   /* compute once */

    return seq[(size_t)n - 1];
}
