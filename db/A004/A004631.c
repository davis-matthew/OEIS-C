/*
 * A004631  Squares written in base 16. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n+1)² written in base 16, then interpreted as decimal
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)(n + 1);
    m = m * m;
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (m > 0) {
        seq_t digit = m % 16;
        result = result + digit * power;
        power = power * 10;
        m = m / 16;
    }
    
    return result;
}
