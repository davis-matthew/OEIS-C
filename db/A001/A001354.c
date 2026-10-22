/*
 * A001354  Coordination sequence for hyperbolic tessellation 3^7 (from
 *          triangle group (2,3,7)).
 * https://oeis.org/A001354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 3*a(n) - a(n-1) for n >= 2; a(0)=1, a(1)=7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 7;
    if (n == 2) return 21;
    
    seq_t prev_prev = 21;
    seq_t prev = 56;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t curr = 3 * prev - prev_prev;
        prev_prev = prev;
        prev = curr;
    }
    
    return prev;
}
