/*
 * A006199  Bessel polynomial {y_n}'(-1).
 * https://oeis.org/A006199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000806(n) + (n-1) * A000806(n-1), where A000806(n) =
 *            -(2n-1)*A000806(n-1) + A000806(n-2)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    
    seq_t A_nm2 = 0;  // A000806(1)
    seq_t A_nm1 = 1;  // A000806(2)
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t A_i = -(2 * (seq_t)i - 1) * A_nm1 + A_nm2;
        A_nm2 = A_nm1;
        A_nm1 = A_i;
    }
    
    return A_nm1 + ((seq_t)n - 1) * A_nm2;
}
