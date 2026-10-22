/*
 * A003688  a(n) = 3*a(n-1) + a(n-2), with a(1)=1 and a(2)=4.
 * https://oeis.org/A003688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2), a(1) = 1, a(2) = 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)4;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = 3 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
