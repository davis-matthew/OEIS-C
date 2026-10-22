/*
 * A006157  a(n+1) = (n-1)*a(n) + n*n!.
 * https://oeis.org/A006157
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-1)/6 * n!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    seq_t coeff = (seq_t)(2 * n - 1);
    
    if (n == 2) {
        return (coeff * factorial) / 6;
    } else {
        return coeff * (factorial / 6);
    }
}
