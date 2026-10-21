/*
 * A085540  a(n) = n*(n + 1)^3.
 * https://oeis.org/A085540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^3
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t s1 = sn + (seq_t)1;      /* n + 1 */
    seq_t s2 = s1 * s1;            /* (n + 1)^2 */
    seq_t s3 = s2 * s1;            /* (n + 1)^3 */
    return sn * s3;                /* n * (n + 1)^3 */
}
