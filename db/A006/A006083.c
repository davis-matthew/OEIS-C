/*
 * A006083  Continued fraction for e/2.
 * https://oeis.org/A006083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=1, a(4)=3, a(5)=1, a(6)=1, a(7)=1, a(8)=3,
 *            then for k>=1: a(6*k+3)=a(6*k+6)=2*k+1, a(6*k+4)=a(6*k+8)=3,
 *            a(6*k+5)=a(6*k+7)=1
 * Range:     u8 n <= 770, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 8) {
        static const seq_t initial[] = {0, 1, 2, 1, 3, 1, 1, 1, 3};
        return initial[(int)n];
    }
    
    idx_t k = (n - 3) / 6;
    idx_t r = (n - 3) % 6;
    
    if (r == 0 || r == 3) {
        return (seq_t)(2 * k + 1);
    } else if (r == 1 || r == 5) {
        return 3;
    } else {
        return 1;
    }
}
