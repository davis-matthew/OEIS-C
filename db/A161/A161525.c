/*
 * A161525  Number of reduced words of length n in the Weyl group A_25.
 * https://oeis.org/A161525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{25} (1 + x + ... + x^k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 55, u128 n <= 325,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The generating polynomial is of degree 325.) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Max degree = 25 * 26 / 2 = 325 */
    static bool ready = false;
    static seq_t coeff[326];
    static idx_t maxdeg = 0;

    if (!ready) {
        coeff[0] = (seq_t)1;   /* start with polynomial 1 */
        maxdeg = 0;

        for (idx_t k = 1; k <= 25; ++k) {
            idx_t newdeg = maxdeg + k;

            /* add the factor (1 + x + ... + x^k) to the current polynomial */
            for (idx_t i = maxdeg; ; --i) {
                seq_t base = coeff[i];
                if (base) {
                    for (idx_t j = 1; j <= k; ++j)
                        coeff[i + j] += base;
                }
                if (i == 0) break;   /* unsigned loop termination */
            }

            maxdeg = newdeg;
        }
        ready = true;
    }

    if (n > maxdeg) return (seq_t)0;
    return coeff[n];
}
