/*
 * A050534  Tritriangular numbers: a(n) = binomial(binomial(n,2),2) =
 *          n*(n+1)*(n-1)*(n-2)/8.
 * https://oeis.org/A050534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n+1)*(n-1)*(n-2)/8
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 409, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)     /* a(0)=a(1)=a(2)=0 */
        return (seq_t)0;

    seq_t nn = (seq_t)n;

    /* Compute the product in two parts to keep intermediate values small */
    seq_t part1 = nn * (nn + 1);        /* n*(n+1)   */
    seq_t part2 = (nn - 1) * (nn - 2);  /* (n-1)*(n-2) */

    seq_t prod = part1 * part2;        /* full product */
    return prod / (seq_t)8;            /* division is exact for n ≥ 3 */
}
