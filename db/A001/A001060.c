/*
 * A001060  a(n) = a(n-1) + a(n-2) with a(0)=2, a(1)=5. Sometimes called the
 *          Evangelist Sequence.
 * https://oeis.org/A001060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 5
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)5;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
