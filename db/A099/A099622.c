/*
 * A099622  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k, k-1)*4^(n-k-1)*(5/4)^k.
 * https://oeis.org/A099622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2) - 20*a(n-3), with a(0) = 0, a(1) =
 *            1, a(2) = 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 8;

    /* a0, a1, a2 represent a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = 0;  /* a(0) */
    seq_t a1 = 1;  /* a(1) */
    seq_t a2 = 8;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term1 = 8 * a2;
        seq_t term2 = 11 * a1;
        seq_t term3 = 20 * a0;
        seq_t next = term1 - term2 - term3;

        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;  /* a(n) */
}
