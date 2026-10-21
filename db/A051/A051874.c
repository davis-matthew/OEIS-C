/*
 * A051874  22-gonal numbers: a(n) = n*(10*n-9).
 * https://oeis.org/A051874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(10*n-9)
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute 10*n*n - 9*n to avoid underflow for n = 0 */
    seq_t t1 = (seq_t)10 * sn * sn;   /* 10 * n^2 */
    seq_t t2 = sn * (seq_t)9;         /* 9 * n   */
    return t1 - t2;
}
