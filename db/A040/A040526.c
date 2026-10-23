/*
 * A040526  Continued fraction for sqrt(550).
 * https://oeis.org/A040526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23, a(n) for n≥1 equals the 16‑term repeating block
 *            (2,4,1,2,1,1,7,4,7,1,1,2,1,4,2,46) of the continued fraction for
 *            sqrt(550)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    /* 16 repeating terms of the continued fraction of sqrt(550) after a(0). */
    static const seq_t period[16] = {
        2, 4, 1, 2, 1, 1, 7, 4,
        7, 1, 1, 2, 1, 4, 2, 46
    };

    /* (n-1) cannot underflow because the n==0 case is handled above. */
    return period[(n - 1) & (16 - 1)];
}
