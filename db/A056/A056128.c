/*
 * A056128  a(n) = (9*n + 11)*binomial(n+10, 10)/11.
 * https://oeis.org/A056128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n + 11)*binomial(n+10, 10)/11
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 221, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute binomial(n+10, 10) */
    seq_t bin = binomial(n + 10, 10);

    /* t = 9*n + 11 */
    seq_t t = ((seq_t)9 * (seq_t)n) + (seq_t)11;

    /* if t divisible by 11 use that division,
       otherwise binomial is guaranteed to be divisible by 11 */
    if (t % (seq_t)11 == (seq_t)0) {
        seq_t q = t / (seq_t)11;
        return q * bin;
    } else {
        return t * (bin / (seq_t)11);
    }
}
