/*
 * A126274  Partial sum of A005915.
 * https://oeis.org/A126274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(n + 1)^2*(n + 2)*(3*n + 2)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 242, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn + (seq_t)1;          /* n + 1 */
    seq_t t2 = nn + (seq_t)2;          /* n + 2 */
    seq_t t3 = (seq_t)3 * nn + (seq_t)2; /* 3*n + 2 */
    /* compute (n+1)^2 * (n+2) * (3*n+2) then divide by 4 */
    seq_t result = t1 * t1 * t2 * t3;
    return result / (seq_t)4;
}
