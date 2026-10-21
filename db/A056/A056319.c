/*
 * A056319  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly three different colors.
 * https://oeis.org/A056319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A056310(n/d) (OEIS A056319) where
 *            A056310(k) = (T_k + T_{ceil(k/2)}) / 2, T_m = 3^m - 3·2^m + 3
 *            The implementation uses the utilities mu_of, seq_pow, and
 *            mul_small
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mu_of, mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

/*  Compute A056310(k) = (T_k + T_{ceil(k/2)})/2  */
static inline seq_t A056310(idx_t k)
{
    /* T_k = 3^k - 3·2^k + 3 */
    seq_t t = seq_pow((seq_t)3, k);
    seq_t twoPow = seq_pow((seq_t)2, k);
    t -= mul_small(twoPow, 3);
    t += (seq_t)3;

    /* T_{ceil(k/2)} */
    idx_t half = (k + 1) / 2;
    seq_t h = seq_pow((seq_t)3, half);
    seq_t twoHalf = seq_pow((seq_t)2, half);
    h -= mul_small(twoHalf, 3);
    h += (seq_t)3;

    return (t + h) / 2;
}

/*  Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* unused sentinel */

    seq_t pos = 0;                          /* positive Möbius contributions */
    seq_t neg = 0;                          /* negative Möbius contributions */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;

        int mu_d = mu_of(d);
        if (mu_d != 0) {
            seq_t term = A056310(e);       /* n/d = e */
            if (mu_d > 0) pos += term; else neg += term;
        }

        if (e != d) {
            int mu_e = mu_of(e);
            if (mu_e != 0) {
                seq_t term = A056310(d);   /* n/e = d */
                if (mu_e > 0) pos += term; else neg += term;
            }
        }
    }

    return pos - neg;                      /* pos >= neg for this sequence */
}
