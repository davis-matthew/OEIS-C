/*
 * A155670  a(n) = 11^n+9^n-1^n.
 * https://oeis.org/A155670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 9^n - 1^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence for b(n) = a(n)+1: b(n) = 20*b(n-1) - 99*b(n-2), b(0)=2, b(1)=20 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t prev0 = (seq_t)2; /* b(0) */
    seq_t prev1 = (seq_t)20; /* b(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * 20 - prev0 * 99;
        prev0 = prev1;
        prev1 = cur;
    }
    return prev1 - (seq_t)1; /* a(n) = b(n) - 1 */
}
