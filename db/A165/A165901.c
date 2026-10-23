/*
 * A165901  a(0)=0, a(1)=1, a(n) = a(n-1) + 2^(n-3)*a(n-2).
 * https://oeis.org/A165901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = a(n-1) + 2^(n-3)*a(n-2) for n >= 3
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;

    seq_t prev2 = 1;     /* a(1) */
    seq_t prev1 = 1;     /* a(2) */
    seq_t pow   = 1;     /* 2^(3-3) = 1 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = prev1 + pow * prev2; /* a(i) */
        prev2 = prev1;
        prev1 = cur;
        pow *= 2;                     /* next power of two */
    }

    return prev1;
}
