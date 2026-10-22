/*
 * A006574  Number of domino n-tuples.
 * https://oeis.org/A006574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) + a(n-4), a(1) = 2, a(2) = 5,
 *            a(3) = 14, a(4) = 38
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 5;
    if (n == 3) return 14;
    if (n == 4) return 38;
    
    seq_t a_nm4 = 2;
    seq_t a_nm3 = 5;
    seq_t a_nm2 = 14;
    seq_t a_nm1 = 38;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t a_n = 3*a_nm1 - a_nm2 + a_nm3 + a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    
    return a_nm1;
}
