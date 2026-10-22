/*
 * A002793  a(n) = 2n*a(n-1) - (n-1)^2*a(n-2).
 * https://oeis.org/A002793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n*a(n-1) - (n-1)^2*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t prev2 = 0;
    seq_t prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t i_minus_1 = (seq_t)(i - 1);
        seq_t curr = 2 * i_seq * prev1 - i_minus_1 * i_minus_1 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
