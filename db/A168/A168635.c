/*
 * A168635  a(n) = n^7*(n + 1)/2.
 * https://oeis.org/A168635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t N = (seq_t)n;

    /* Compute N^7 by successive multiplication. */
    seq_t pow = N;                  /* N^1 */
    for (idx_t i = 0; i < 6; ++i)    /* multiply 6 more times → N^7 */
        pow *= N;

    /* Now compute (N^7 * (N + 1)) / 2 without overflow.
       At least one of pow or (N + 1) is even, so divide the even factor
       by 2 before performing the final multiplication. */
    seq_t np1 = N + 1;
    if ((pow & 1) == 0) {           /* pow is even */
        pow >>= 1;                  /* divide pow by 2 */
        return pow * np1;
    } else {                        /* (N + 1) must be even */
        np1 >>= 1;                  /* divide (N + 1) by 2 */
        return pow * np1;
    }
}
