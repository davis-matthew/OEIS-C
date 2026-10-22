/*
 * A001360  Crystal ball sequence for hyperbolic tessellation 3^7 (from
 *          triangle group (2,3,7)).
 * https://oeis.org/A001360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), a(0) = 1, a(1) = 8, a(2) =
 *            29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)29;
    
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)29;
    seq_t a3;
    
    for (idx_t i = 3; i <= n; i++) {
        a3 = (seq_t)4 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    
    return a2;
}
