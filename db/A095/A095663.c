/*
 * A095663  Eighth column (m=7) of (1,3)-Pascal triangle A095660.
 * https://oeis.org/A095663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6, 6) * (n+21) / 7
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+6, 6) using the provided utility */
    seq_t bin = binom(n + 6, 6);

    /* Extract the factor 7 from bin or from (n+21) to avoid overflow */
    seq_t g = gcd(bin, (seq_t)7);           /* g is 1 or 7 */
    seq_t bin_div = bin / g;
    seq_t factor  = (seq_t)7 / g;           /* factor is 1 or 7 */

    /* Now (n+21)/factor is guaranteed to be integer */
    seq_t n21 = ((seq_t)n + 21) / factor;

    return bin_div * n21;
}
