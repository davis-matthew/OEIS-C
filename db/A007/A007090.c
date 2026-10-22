/*
 * A007090  Numbers in base 4.
 * https://oeis.org/A007090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = interpret in base 10 the base 4 representation of n
 * Range:     u8 n <= 47, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power = 1;
    
    while (n > 0) {
        seq_t digit = (seq_t)(n % 4);
        result += digit * power;
        power *= 10;
        n /= 4;
    }
    
    return result;
}
