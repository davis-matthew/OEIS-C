/*
 * A001053  a(n+1) = n*a(n) + a(n-1) with a(0)=1, a(1)=0.
 * https://oeis.org/A001053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n+1) = n*a(n) + a(n-1), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    seq_t prev_prev = 1;
    seq_t prev = 0;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = (seq_t)(i - 1) * prev + prev_prev;
        prev_prev = prev;
        prev = curr;
    }
    
    return curr;
}
