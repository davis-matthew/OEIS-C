/*
 * A096222  Number of different rectangles when a piece of paper is folded n
 *          times in alternate directions.
 * https://oeis.org/A096222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a*b*(a+1)*(b+1)/4 with a = 2^⌊n/2⌋, b = 2^⌈n/2⌉
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute a = 2^floor(n/2) */
    seq_t a = 1;
    for (idx_t i = 0; i < (n >> 1); ++i)
        a <<= 1;

    /* compute b = 2^ceil(n/2) = 2^((n+1)/2) */
    seq_t b = 1;
    for (idx_t i = 0; i < ((n + 1) >> 1); ++i)
        b <<= 1;

    /* compute (a*(a+1))/2 safely
       one of a and a+1 is even */
    seq_t tA;
    if ((a & 1) == 0)
        tA = (a >> 1) * (a + 1);
    else
        tA = a * (((a + 1) >> 1));

    seq_t tB;
    if ((b & 1) == 0)
        tB = (b >> 1) * (b + 1);
    else
        tB = b * (((b + 1) >> 1));

    /* final result */
    return tA * tB;
}
