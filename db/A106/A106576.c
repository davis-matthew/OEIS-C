/*
 * A106576  Period 20. Sequence gives last digit of A106157, starting from the
 *          first positive term.
 * https://oeis.org/A106576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) satisfies a(0) = 1 a(n) = coeff[n] + a(n-2) - a(n-4) +
 *            a(n-6) - a(n-8) + a(n-10) - a(n-12) + a(n-14) - a(n-16) +
 *            a(n-18), where coeff[n] is the coefficient of x^n in A(x) = 1 +
 *            2x + 3x^4 + 7x^12 + 4x^13 + 5x^8 + 9x^16 + 8x^17 + 6x^5
 *            (coeff(other) = 0). This recurrence produces the last digit...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized sequence values   */
static idx_t cache_len = 0;      /* number of computed elements */

/* coefficients of A(x) up to degree 17 (higher indices are zero) */
static const int coeff[18] = {
    1, 2, 0, 0, 3, 6, 0, 0, 5, 0,
    0, 0, 7, 4, 0, 0, 9, 8
};

seq_t a(idx_t n)
{
    /* enlarge cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t sum = (seq_t)0;

            /* add the A(x) coefficient when it exists */
            if (k < 18)
                sum += (seq_t)coeff[(size_t)k];

            /* add/subtract previous values according to B(x) = 1−x²+x⁴−…+x¹⁸ */
            int sign = 1;                     /* + for offset 2 */
            for (int offset = 2; offset <= 18; offset += 2) {
                if (k >= (idx_t)offset) {
                    if (sign > 0)
                        sum += cache[k - offset];
                    else
                        sum -= cache[k - offset];
                }
                sign = -sign;                /* alternate signs */
            }
            cache[k] = sum;
        }
        cache_len = new_len;
    }
    return cache[n];
}
