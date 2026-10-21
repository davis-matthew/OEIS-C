/*
 * A214113  Number of 0..3 colorings of a 2X(n+1) array circular in the n+1
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+22*a(n-2)-4*a(n-3)-21*a(n-4); Initial values:
 *            a(1)=4, a(2)=11, a(3)=111, a(4)=670
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unspecified, offset starts at 1 */
    if (n == 1) return 4;
    if (n == 2) return 11;
    if (n == 3) return 111;
    if (n == 4) return 670;

    seq_t prev4 = 4;   /* a(n-4) */
    seq_t prev3 = 11;  /* a(n-3) */
    seq_t prev2 = 111; /* a(n-2) */
    seq_t prev1 = 670; /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = 4 * prev1 + 22 * prev2 - 4 * prev3 - 21 * prev4;
        /* rotate the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
