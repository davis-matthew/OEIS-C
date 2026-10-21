/*
 * A098077  a(n) = n^2*(n+1)*(2*n+1)/3.
 * https://oeis.org/A098077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n+1) * (2*n+1) / 3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn    = (seq_t)n;            /* copy n as seq_t */
    seq_t n2    = nn * nn;             /* n^2 */
    seq_t n1    = nn + 1;              /* n + 1 */
    seq_t two_n = (nn << 1) + 1;       /* 2*n + 1 (<< 1 = 2*nn) */
    seq_t prod  = n2 * n1;             /* n^2 * (n+1) */
    prod        = prod * two_n;        /* * (2*n+1) */
    prod        = prod / 3;            /* divide by 3 (exact division) */
    return prod;
}
