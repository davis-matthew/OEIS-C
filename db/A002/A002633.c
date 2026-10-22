/*
 * A002633  Related to discordant permutations.
 * https://oeis.org/A002633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) - (2n-5)*a(n-1) + (n-1)*(n-4)*a(n-2) + (n-1)*(n-2)*a(n-3) =
 *            0 Rearranged: a(n) = (2n-5)*a(n-1) - (n-1)*(n-4)*a(n-2) -
 *            (n-1)*(n-2)*a(n-3) Base cases: a(0) = 1, a(1) = -3, a(2) = 5
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)5;
    
    seq_t a_nm3 = (seq_t)1;
    seq_t a_nm2 = (seq_t)-3;
    seq_t a_nm1 = (seq_t)5;
    seq_t a_n;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t coef1 = 2*ni - 5;
        seq_t coef2 = (ni - 1) * (ni - 4);
        seq_t coef3 = (ni - 1) * (ni - 2);
        
        a_n = coef1 * a_nm1 - coef2 * a_nm2 - coef3 * a_nm3;
        
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    
    return a_n;
}
