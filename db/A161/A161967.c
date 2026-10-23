/*
 * A161967  E.g.f. satisfies: A(x) = exp( x*exp( x*A'(x)/A(x) ) ), where
 *          A'(x)/A(x) = d/dx log(A(x)).
 * https://oeis.org/A161967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th term of OEIS A161967, defined by the e.g.f.
 *            equation A(x) = exp( x * exp( x * A'(x)/A(x) ) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* global cached tables for the sequence */
static seq_t *A_cache = NULL;  /* a(n) */
static seq_t *B_cache = NULL;  /* b(n) = coeff of ln A */
static seq_t *E_cache = NULL;  /* e(n) = coeff of exp( x*A'(x)/A ) */

static size_t cache_cap = 0;   /* allocated size of tables       */
static size_t cache_used = 0;  /* number of indices currently stored (>=1, first index is 0) */

/* ensure the internal tables are large enough to hold values up to idx */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    A_cache = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
    B_cache = (seq_t *)realloc(B_cache, new_cap * sizeof(seq_t));
    E_cache = (seq_t *)realloc(E_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Populate the tables up to index n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if ((idx_t)cache_used > n)
        return;

    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    ensure_capacity(needed);

    /* initialise the base case if the table is empty */
    if (cache_used == 0) {
        A_cache[0] = 1;
        B_cache[0] = 0;
        E_cache[0] = 1;
        cache_used = 1;
    }

    /* compute values from the current end up to n */
    for (size_t i = cache_used; i < needed; ++i) {
        idx_t i_idx = (idx_t)i;

        /* --- compute a[i] --- */
        seq_t sum_a = 0;
        seq_t binom = 1;
        for (idx_t k = 1; k <= i_idx; ++k) {
            seq_t term = binom;
            term *= (seq_t)k;
            term *= E_cache[k - 1];
            term *= A_cache[i_idx - k];
            sum_a += term;

            if (k < i_idx) {
                seq_t num = (seq_t)(i_idx - k);
                seq_t den = (seq_t)k;
                binom = (binom * num) / den;
            }
        }
        A_cache[i] = sum_a;

        /* --- compute b[i] for i > 0 --- */
        if (i_idx > 0) {
            seq_t sum_b = 0;
            binom = 1;
            for (idx_t k = 1; k <= i_idx - 1; ++k) {
                seq_t term = binom;
                term *= B_cache[k];
                term *= A_cache[i_idx - k];
                sum_b += term;

                seq_t num = (seq_t)(i_idx - k);
                seq_t den = (seq_t)k;
                binom = (binom * num) / den;
            }
            B_cache[i] = A_cache[i] - sum_b;
        }

        /* --- compute e[i] for i >= 1 --- */
        if (i_idx >= 1) {
            seq_t sum_e = 0;
            binom = 1;
            for (idx_t k = 1; k <= i_idx; ++k) {
                seq_t term = binom;
                term *= (seq_t)k;
                term *= B_cache[k];
                term *= E_cache[i_idx - k];
                sum_e += term;

                if (k < i_idx) {
                    seq_t num = (seq_t)(i_idx - k);
                    seq_t den = (seq_t)k;
                    binom = (binom * num) / den;
                }
            }
            E_cache[i] = sum_e;
        }
    }

    cache_used = needed;
}

/* Public function required by the judge */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return A_cache[n];
}
