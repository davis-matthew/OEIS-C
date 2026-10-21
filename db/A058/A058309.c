/*
 * A058309  a(n) = (n+3)*a(n-1) + a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A058309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (n+3)*a(n-1) + a(n-2) (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 0;
    if (n == 0) return a0;
    seq_t a1 = 1;
    if (n == 1) return a1;

    seq_t prev2 = a0;  /* a(n-2) */
    seq_t prev1 = a1;  /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = (i+3)*a(i-1) + a(i-2) */
        current = ((seq_t)(i + 3)) * prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
