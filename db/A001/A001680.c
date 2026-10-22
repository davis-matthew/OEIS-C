/*
 * A001680  The partition function G(n,3).
 * https://oeis.org/A001680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + (n-1)*(n-2)/2*a(n-3), a(0) = 1,
 *            a(1) = 1, a(2) = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    
    seq_t a_nm3 = 1;
    seq_t a_nm2 = 1;
    seq_t a_nm1 = 2;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t im1 = ni - 1;
        seq_t im2 = ni - 2;
        
        seq_t a_n = a_nm1 + im1 * a_nm2 + im1 * im2 / 2 * a_nm3;
        
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    
    return a_nm1;
}
