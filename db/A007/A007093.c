/*
 * A007093  Numbers in base 7.
 * https://oeis.org/A007093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 0; a(n) = 10*a(n/7) if n≡0(mod 7), else a(n) = a(n-1)+1
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t multiplier = 1;
    
    while (n > 0) {
        seq_t digit = (seq_t)(n % 7);
        result = result + digit * (seq_t)multiplier;
        multiplier *= 10;
        n /= 7;
    }
    
    return result;
}
