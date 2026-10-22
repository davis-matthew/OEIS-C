/*
 * A004630  Squares written in base 12. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = base-12 representation of n^2, interpreted as a decimal
 *            number
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_squared = n_seq * n_seq;
    
    seq_t result = 0;
    seq_t power = 1;
    
    seq_t temp = n_squared;
    while (temp > 0) {
        seq_t digit = temp % 12;
        result = result + digit * power;
        power = power * 10;
        temp = temp / 12;
    }
    
    return result;
}
