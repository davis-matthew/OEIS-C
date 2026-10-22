/*
 * A006085  Continued fraction for e/4.
 * https://oeis.org/A006085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   First eight terms are 0, 1, 2, 8, 3, 1, 1, 1; then a(8k)=k,
 *            a(8k+1)=7, a(8k+2)=1, a(8k+3)=k, a(8k+4)=2, a(8k+5)=1,
 *            a(8k+6)=1, a(8k+7)=1
 * Range:     u8 n <= 2047, u16 n <= 9159, u32 n <= 9159, u64 n <= 9159,
 *            u128 n <= 9159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t initial[8] = {0, 1, 2, 8, 3, 1, 1, 1};
    
    if (n < 8) {
        return initial[n];
    }
    
    idx_t k = n / 8;
    idx_t r = n % 8;
    
    if (r == 0 || r == 3) {
        return (seq_t)k;
    } else if (r == 1) {
        return 7;
    } else if (r == 4) {
        return 2;
    } else {
        return 1;
    }
}
