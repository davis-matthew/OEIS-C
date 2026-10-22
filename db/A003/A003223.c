/*
 * A003223  Erroneous version of A006841.
 * https://oeis.org/A003223
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 4*a(n-1) + 21*a(n-2) - 48*a(n-3), a(3) = 1, a(4) = 2,
 *            a(5) = 4
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) return 1;
    if (n == 4) return 2;
    if (n == 5) return 4;
    
    seq_t a_nm3 = 1, a_nm2 = 2, a_nm1 = 4;
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t a_n = 4 * a_nm1 + 21 * a_nm2 - 48 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    
    return a_nm1;
}
