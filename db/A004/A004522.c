/*
 * A004522  Generalized nim sum n + n in base 12.
 * https://oeis.org/A004522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = generalized nim sum n + n in base 12 (digit-wise addition
 *            mod 12 with no carries)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t multiplier = 1;
    seq_t m = (seq_t)n;
    
    while (m > 0) {
        seq_t digit = m % 12;
        seq_t new_digit = (2 * digit) % 12;
        result += new_digit * multiplier;
        multiplier *= 12;
        m /= 12;
    }
    
    return result;
}
