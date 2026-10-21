/*
 * A052798  G.f. A(x) satisfies: A(x) = exp( Sum_{k>=1} (-1)^(k+1) * A(x^k)^5
 *          * x^k / k ).
 * https://oeis.org/A052798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* G.f. A(x) satisfies: A(x) = exp( Sum_{k>=1} (-1)^(k+1) * A(x^k)^5 * x^k / k ).
   Recurrence: let s[n] = Σ_{d|n} (-1)^(d+1) * (n/d) * c5[n/d-1] (c5 is the coefficient of A^5).
   Then for n≥1: a[n] = (1/n) * Σ_{k=1}^{n} s[k] * a[n-k].
   The implementation stores all sequence values A[n] and the five
   consecutive convolutions
        c2[n] = Σ_{i=0}^{n} A[i]*A[n-i],
        c3[n] = Σ_{i=0}^{n} A[i]*c2[n-i],
        c4[n] = Σ_{i=0}^{n} A[i]*c3[n-i],
        c5[n] = Σ_{i=0}^{n} A[i]*c4[n-i],
   updating them incrementally.  The function works for any
   seq_t/idx_t pair that satisfies the build constraints. */

/* library helpers */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *A     = NULL;
static seq_t *c2    = NULL;
static seq_t *c3    = NULL;
static seq_t *c4    = NULL;
static seq_t *c5    = NULL;
static seq_t *s     = NULL;
static size_t cap   = 0;          /* currently allocated size of arrays   */
static idx_t computed = 0;        /* largest index that has been computed */

/* Grow all arrays so that indices up to 'n' are valid (inclusive). */
static void grow_to(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        A  = seq_realloc(A , cap, needed);
        c2 = seq_realloc(c2, cap, needed);
        c3 = seq_realloc(c3, cap, needed);
        c4 = seq_realloc(c4, cap, needed);
        c5 = seq_realloc(c5, cap, needed);
        s  = seq_realloc(s , cap, needed);
        /* zero‑initialize the newly added elements */
        for (size_t i = cap; i < needed; ++i) {
            A[i] = 0; c2[i] = 0; c3[i] = 0; c4[i] = 0; c5[i] = 0; s[i] = 0;
        }
        cap = needed;
    }
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    grow_to(n);

    /* initialize the base case a(0)=1, c2[0]…c5[0]=1, s[0] unused */
    if (computed == 0 && A[0] == 0) {
        A[0]  = 1;
        c2[0] = 1;
        c3[0] = 1;
        c4[0] = 1;
        c5[0] = 1;
    }

    /* compute any missing terms up to 'n' */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* ----- compute s[m] ----- */
        seq_t pos = 0, neg = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* first divisor d */
                idx_t d1 = d;
                idx_t d2 = m / d;
                seq_t coeff1 = c5[m / d1 - 1];
                seq_t mult1  = (seq_t)(m / d1);
                seq_t term1  = coeff1 * mult1;
                if (d1 & 1) pos += term1; else neg += term1;

                /* second divisor, if distinct */
                if (d1 != d2) {
                    seq_t coeff2 = c5[m / d2 - 1];
                    seq_t mult2  = (seq_t)(m / d2);
                    seq_t term2  = coeff2 * mult2;
                    if (d2 & 1) pos += term2; else neg += term2;
                }
            }
        }
        /* pos >= neg by construction of the formula */
        s[m] = pos - neg;

        /* ----- compute a[m] ----- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += s[k] * A[m - k];
        }
        seq_t a_m = sum / (seq_t)m;
        A[m] = a_m;

        /* ----- update convolutions ----- */
        /* c2[m] = Σ_{i=0}^{m} A[i] * A[m-i] */
        seq_t val2 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            val2 += A[i] * A[m - i];
        }
        c2[m] = val2;

        /* c3[m] = Σ_{i=0}^{m} A[i] * c2[m-i] */
        seq_t val3 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            val3 += A[i] * c2[m - i];
        }
        c3[m] = val3;

        /* c4[m] = Σ_{i=0}^{m} A[i] * c3[m-i] */
        seq_t val4 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            val4 += A[i] * c3[m - i];
        }
        c4[m] = val4;

        /* c5[m] = Σ_{i=0}^{m} A[i] * c4[m-i] */
        seq_t val5 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            val5 += A[i] * c4[m - i];
        }
        c5[m] = val5;
    }

    computed = n;
    return A[n];
}
