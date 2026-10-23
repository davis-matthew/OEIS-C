/*
 * A100145  Structured great rhombicosidodecahedral numbers.
 * https://oeis.org/A100145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (1 + 22*(n-1) + 37*(n-1)^2)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 488, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1, but guard against 0 */

    /* Convert index to seq_t to avoid overflow on the index itself */
    seq_t N = n;
    seq_t m = N - 1;            /* m = n-1 */

    /* Compute the polynomial part: 1 + 22*m + 37*m*m */
    seq_t poly = 1 + 22 * m + 37 * m * m;

    /* Final result */
    return N * poly;
}
