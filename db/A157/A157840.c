/*
 * A157840  a(n) = 103680000*n^2 - 174211200*n + 73180801.
 * https://oeis.org/A157840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 103680000*n^2 - 174211200*n + 73180801
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    if (n == 1) return 2649601;
    if (n == 2) return 139478401;
    if (n == 3) return 483667201;

    seq_t a0 = 2649601;    /* a(n-3)   */
    seq_t a1 = 139478401;  /* a(n-2)   */
    seq_t a2 = 483667201;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_next = 3 * a2 - 3 * a1 + a0;  /* a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
