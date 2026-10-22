/*
 * A000779  a(n) = 2*(2n-1)!!-(n-1)!*2^(n-1), where (2n-1)!! is A001147(n).
 * https://oeis.org/A000779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(2n-1)!! - (n-1)!*2^(n-1), where (2n-1)!! is A001147(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    
    seq_t double_factorial = 1;
    for (seq_t i = 1; i <= sn; i++) {
        double_factorial *= (2 * i - 1);
    }
    
    seq_t factorial_part = 1;
    for (seq_t i = 1; i < sn; i++) {
        factorial_part *= i;
    }
    
    seq_t power_of_2 = 1;
    for (seq_t i = 1; i < sn; i++) {
        power_of_2 *= 2;
    }
    
    return 2 * double_factorial - factorial_part * power_of_2;
}
