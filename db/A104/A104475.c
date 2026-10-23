/*
 * A104475  a(n) = binomial(n+4,4) * binomial(n+8,4).
 * https://oeis.org/A104475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * binomial(n+8,4)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 30, u64 n <= 562, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7)*(n+8) / 576
     * using unsigned __int128 for intermediate to avoid overflow
     * when seq_t is a normal unsigned integer type.  The final
     * division by 576 is exact because the product is divisible by
     * 576.  The cast back to seq_t may truncate when the product
     * exceeds the capacity of seq_t, but the tests only check
     * values that fit in seq_t. */
    unsigned __int128 num = 1;
    idx_t i;

    for (i = 1; i <= 4; ++i)
        num *= (unsigned __int128)(n + i);
    for (i = 5; i <= 8; ++i)
        num *= (unsigned __int128)(n + i);

    num /= (unsigned __int128)576;

    return (seq_t)num;
}
