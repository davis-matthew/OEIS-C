/*
 * A157984  a(n) = n!/(7 * 20!).
 * https://oeis.org/A157984
 *
 * Offset:    21    (a(21) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!/(7 * 20!) (for n ≥ 21)
 * Range:     u8 n <= 22, u16 n <= 24, u32 n <= 27, u64 n <= 34, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 21) {
        /* undefined for indices below the OEIS offset */
        return (seq_t)0;
    }

    /* 21 = 7 * 3, so divide by 7 first */
    seq_t result = (seq_t)3;          /* equivalent to 21/7 */

    /* multiply by the remaining factors 22 … n */
    for (idx_t k = 22; k <= n; ++k) {
        result *= (seq_t)k;
    }

    return result;
}
