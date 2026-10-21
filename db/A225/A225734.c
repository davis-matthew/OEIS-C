/*
 * A225734  Numbers n such that n < d(n)^(26/10), where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A225734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers n such that n^5 < d(n)^13 (i.e., n < d(n)^(26/10))
 * Range:     u8 n <= 105, u16 n <= 1380, u32 n <= 2046, u64 n <= 2046,
 *            u128 n <= 2046, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to compute a^e for unsigned 128‑bit types */
static inline unsigned __int128 pow_uint128(unsigned __int128 a, unsigned e)
{
    unsigned __int128 r = 1;
    while (e) {
        if (e & 1) r *= a;
        a *= a;
        e >>= 1;
    }
    return r;
}

/* Static state for caching divisor counts and sequence terms */
static uint32_t *div_counts = NULL;   /* divisor counting array   */
static idx_t   d_limit    = 0;        /* size of div_counts array */
static seq_t  *seq_terms   = NULL;   /* stored sequence terms    */
static size_t  term_cnt   = 0;        /* number of terms stored   */
static size_t  term_cap   = 0;        /* allocated capacity       */
static idx_t   next_n     = 2;        /* next candidate to test   */

/* Build divisor counts for all numbers up to 'limit' inclusively */
static void build_div_counts(idx_t limit)
{
    uint32_t *tmp = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    for (idx_t i = 1; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            ++tmp[j];
    free(div_counts);
    div_counts = tmp;
    d_limit    = limit;
}

/* Append a term to the dynamic sequence array */
static void append_term(seq_t val)
{
    if (term_cnt == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 16;
        seq_terms = (seq_t *)realloc(seq_terms, term_cap * sizeof(seq_t));
    }
    seq_terms[term_cnt++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initial divisor array build if necessary */
    if (div_counts == NULL) build_div_counts(2000000);

    /* Generate terms until we have at least n of them */
    while (term_cnt < n) {
        /* Expand divisor table if we run out of precomputed space */
        if (next_n > d_limit) {
            build_div_counts(d_limit * 2);
        }

        uint32_t dval = div_counts[next_n];

        /* Compute n^5 and d(val)^13 with 128‑bit integers */
        unsigned __int128 n5  = pow_uint128((unsigned __int128)next_n, 5);
        unsigned __int128 d13 = pow_uint128((unsigned __int128)dval, 13);

        if (n5 < d13)
            append_term((seq_t)next_n);

        ++next_n;
    }

    /* Return the n‑th term (offset 1) */
    return seq_terms[n - 1];
}
