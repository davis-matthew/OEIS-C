/*
 * A008384  Crystal ball sequence for A_4 lattice.
 * https://oeis.org/A008384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1 + 5*n*(n+1)*(7*n^2+7*n+10)/12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t ns = (seq_t)n;
    seq_t ns1 = ns + 1;          /* n + 1 */
    seq_t ns2 = ns * ns;         /* n^2 */
    seq_t t4  = 7 * ns2 + 7 * ns + 10;   /* 7*n^2 + 7*n + 10 */
    seq_t num = 5 * ns * ns1 * t4;  /* 5*n*(n+1)*(7*n^2+7*n+10) */
    return 1 + num / 12;
}
