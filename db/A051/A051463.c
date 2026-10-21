/*
 * A051463  Molien series for group G_{1,3}^{8} of order 4608.
 * https://oeis.org/A051463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the coefficient of the rational generating function
 *            (5*x^12 + 51*x^11 + 190*x^10 + 400*x^9 + 846*x^8 + 947*x^7 +
 *            882*x^6 + 795*x^5 + 304*x^4 + 152*x^3 + 36*x^2 - x + 1) / ((x -
 *            1)^6 * (x^2 + x + 1)^4)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies a linear recurrence of order 14 with
   constant coefficients:
      ∑_{k=0..14} f_k · a(n-k) = rhs(n),
   where f = [1, -2, 1, -4, 8, -4, 6, -12, 6, -4, 8, -4, 1, -2, 1]
   and rhs(0..12) = [1, -1, 36, 152, 304, 795, 882, 947,
                     846, 400, 190, 51, 5], rhs(n)=0 for n≥13.
   The implementation uses memoisation to compute terms up to the
   requested index in O(n) time. */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const int f[15] = {
    1,  -2,   1,  -4,   8,  -4,   6, -12,   6,  -4,   8,  -4,   1,  -2,   1
};

static const int N[13] = {
    1,  -1,  36, 152, 304, 795, 882, 947,
    846, 400, 190, 51, 5
};

/* Computes the n-th term of sequence A051463 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* Allocate or expand cache to hold terms up to n */
    if (n + 1 > cached) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();          /* Out of memory */
        cache = tmp;

        /* Compute new terms */
        for (size_t i = cached; i <= n; ++i) {
            if (i < 14) {   /* Use signed arithmetic for small indices */
                int64_t sum = 0;
                for (int k = 1; k <= 14 && k <= (int)i; ++k) {
                    sum += (int64_t)f[k] * (int64_t)cache[i - k];
                }
                int64_t rhs = (i <= 12) ? N[i] : 0;
                int64_t diff = rhs - sum;
                cache[i] = (seq_t)diff;
            } else {        /* Use unsigned arithmetic for larger indices */
                seq_t pos = 0, neg = 0;
                for (int k = 1; k <= 14; ++k) {
                    int coeff = f[k];
                    if (coeff > 0)
                        pos += mul_small(cache[i - k], (unsigned)coeff);
                    else
                        neg += mul_small(cache[i - k], (unsigned)(-coeff));
                }
                cache[i] = neg - pos;   /* neg > pos always for i≥14 */
            }
        }
        cached = newsize;
    }
    return cache[n];
}
