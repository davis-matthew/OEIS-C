/*
 * A169510  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A169510 satisfies the linear recurrence * * a(0) = 1 * for n ≥ 1
 *            : * a(n) = c(n) + 15 * Σ_{k=1..33} a(n-k) – 120 * a(n-34) * *
 *            where * c(0) = 1, c(34) = 1 * c(n) = 2 for 1 ≤ n ≤ 33 * c(n) = 0
 *            otherwise * * The recurrence follows from the rational
 *            generating function * given in the OEIS entry. It...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External linkage required by the harness. */
seq_t a(idx_t n)
{
    /* Persistent cache of computed terms. */
    static seq_t *values   = NULL;   /* pointer to the sequence array */
    static size_t cache_len = 0;    /* number of terms stored (max index + 1) */

    /* Expand the cache if necessary. */
    if ((size_t)n + 1 > cache_len) {
        /* Allocate or enlarge the array. */
        values = (seq_t*)realloc(values, ((size_t)n + 1) * sizeof(seq_t));
        if (!values) return (seq_t)0; /* allocation failure – unlikely */

        /* Initialize any newly allocated elements to zero.   (Unnecessary
         * for POD types, but safe for arbitrary-precision types that
         * have a default constructor.) */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            values[i] = (seq_t)0;
        }

        /* Base term a(0) = 1. */
        if (cache_len == 0) {
            values[0] = (seq_t)1;
            cache_len = 1;
        }

        /* Compute terms up to n. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* Sum of the last 33 terms. */
            seq_t sumPrev = (seq_t)0;
            for (int k = 1; k <= 33 && i >= k; ++k) {
                sumPrev += values[i - k];
            }

            /* Constant term c(n). */
            seq_t c;
            if (i == 0 || i == 34) {
                c = (seq_t)1;
            } else if (i <= 33) {
                c = (seq_t)2;
            } else {
                c = (seq_t)0;
            }

            /* 15 * Σ a(n-k). */
            seq_t term1 = (seq_t)15 * sumPrev;

            /* 120 * a(n-34). */
            seq_t term2 = (seq_t)0;
            if (i >= 34) {
                term2 = (seq_t)120 * values[i - 34];
            }

            values[i] = c + term1 - term2;
        }

        cache_len = (size_t)n + 1;
    }

    return values[n];
}
