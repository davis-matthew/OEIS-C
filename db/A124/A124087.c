/*
 * A124087  9th column of Catalan triangle A009766.
 * https://oeis.org/A124087
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,8) - C(n,6) (for n >= 15)
 * Range:     u8 none, u16 n <= 15, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, used only if needed by the build */

/* Multiplicative binomial coefficient calculated in seq_t.
   Assumes seq_t supports * and / with exact integer division. */
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use the smaller k for efficiency */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;          /* division is exact for binomial */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 15) return 0;              /* below the OEIS offset; value not defined */
    seq_t c8 = binom_calc(n, 8);
    seq_t c6 = binom_calc(n, 6);
    return c8 - c6;
}
