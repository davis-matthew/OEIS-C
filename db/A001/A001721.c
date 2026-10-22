/*
 * A001721  Generalized Stirling numbers, [n+6,6]_5.
 * https://oeis.org/A001721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 9)*a(n-1) - (n+4)^2*a(n-2) Initial values: a(0) =
 *            1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)11;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        idx_t i_plus_4 = i + 4;
        seq_t c1 = (seq_t)(2 * i + 9);
        seq_t c2 = (seq_t)i_plus_4 * (seq_t)i_plus_4;
        curr = c1 * prev1 - c2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
