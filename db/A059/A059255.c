/*
 * A059255  Both sum of n+1 consecutive squares and sum of the immediately
 *          following n consecutive squares.
 * https://oeis.org/A059255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = n*(n + 1)*(2*n + 1)*(12*n^2 + 12*n + 1)/6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);            /* n*(n+1) */
    prod = prod * (2 * nn + 1);            /* * (2n+1) */
    seq_t inner = 12 * nn * nn + 12 * nn + 1; /* 12n^2 + 12n + 1 */
    prod = prod * inner;                  /* * (12n^2+12n+1) */
    prod = prod / 6;                      /* exact division by 6 */
    return prod;
}
