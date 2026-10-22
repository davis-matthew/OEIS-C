/*
 * A002742  Logarithmic numbers.
 * https://oeis.org/A002742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)*n*a(n-2) + (n-1)*n*a(n-3) - a(n-1) +
 *            2*(-1)^(n-3)*n, with a(1)=2, a(2)=-6, a(3)=24
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)-6;
    if (n == 3) return (seq_t)24;
    
    seq_t a_nm3 = (seq_t)2;
    seq_t a_nm2 = (seq_t)-6;
    seq_t a_nm1 = (seq_t)24;
    
    for (idx_t m = 4; m <= n; m++) {
        seq_t m_seq = (seq_t)m;
        seq_t coeff = (m_seq - 1) * m_seq;
        seq_t a_m = coeff * a_nm2 + coeff * a_nm3 - a_nm1;
        
        if ((m - 3) % 2 == 0) {
            a_m = a_m + 2 * m_seq;
        } else {
            a_m = a_m - 2 * m_seq;
        }
        
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_m;
    }
    
    return a_nm1;
}
