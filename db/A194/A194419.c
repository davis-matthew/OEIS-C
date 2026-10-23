/*
 * A194419  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) < 0, where
 *          r=sqrt(5) and < > denotes fractional part.
 * https://oeis.org/A194419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m for the m-th integer such that * Sum_{k=1..m} (
 *            {1/3+k*r} - {k*r} ) < 0 * where r = sqrt(5) and {x} is the
 *            fractional part of x. * * The sequence is defined by m when the
 *            number of k≤m with * fractional part of k*sqrt(5) < 2/3 is
 *            strictly less than (2/3)*m. * * The function below generat...
 * Range:     u8 none, u16 n <= 3, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
/* Include the utility providing floor_sqrt() */
#include "../utils/floor_sqrt.h"

static seq_t *terms = NULL;      /* memoised list of sequence values   */
static size_t term_cnt = 0;       /* number of stored terms             */
static size_t term_cap = 0;       /* current capacity of terms[]        */
static idx_t last_m = 0;          /* last integer m processed           */

/* Ensure capacity for at least 'need' terms */
static void ensure_terms_capacity(size_t need)
{
    if (term_cap >= need)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 1024;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Out of memory – aborting (unlikely for the expected sizes) */
        abort();
    }
    terms = new_ptr;
    term_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n < 1, return 0 as sentinel */

    if (term_cnt < (size_t)n) {
        /* We must compute more terms until we reach index n */
        ensure_terms_capacity(n);

        idx_t k = last_m + 1;         /* next integer to test          */
        idx_t sumI = 0;               /* running count of indicator=1   */

        /* If we had previously processed some terms, restore sumI count */
        if (term_cnt > 0) {
            /* recompute sumI up to last_m; this is O(last_m) only the first time */
            idx_t m = 1;
            sumI = 0;
            for (; m <= last_m; ++m) {
                /* Recalculate indicator for m */
                seq_t k_sq = (seq_t)m * (seq_t)m;
                seq_t s = k_sq * (seq_t)5;
                seq_t q = floor_sqrt(s);                   /* ⌊k*sqrt5⌋ */

                seq_t num = k_sq * (seq_t)45;
                seq_t t   = (seq_t)3 * q + (seq_t)2;
                seq_t rhs = t * t;
                if (num < rhs)
                    ++sumI;
            }
        }

        for (k = last_m + 1; term_cnt < (size_t)n; ++k) {
            /* Compute floor(k*sqrt5) */
            seq_t k_sq = (seq_t)k * (seq_t)k;
            seq_t s = k_sq * (seq_t)5;
            seq_t q = floor_sqrt(s);

            /* Indicator test: 45*k^2 < (3*q+2)^2  */
            seq_t num = k_sq * (seq_t)45;
            seq_t t   = (seq_t)3 * q + (seq_t)2;
            seq_t rhs = t * t;
            if (num < rhs)
                ++sumI;

            /* Condition for term: 3*sumI < 2*k */
            if ((seq_t)(3 * sumI) < (seq_t)(2 * k)) {
                terms[term_cnt++] = (seq_t)k;
            }
        }

        last_m = k - 1;
    }

    return terms[n - 1];
}
