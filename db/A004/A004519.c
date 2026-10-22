/*
 * A004519  Generalized nim sum n + n in base 9.
 * https://oeis.org/A004519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = generalized nim sum n + n in base 9; write n in base 9,
 *            add each digit to itself mod 9 with no carries
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power = 1;
    seq_t temp = (seq_t)n;
    
    while (temp > 0) {
        seq_t digit = temp % 9;
        seq_t new_digit = (2 * digit) % 9;
        result += new_digit * power;
        power *= 9;
        temp /= 9;
    }
    
    return result;
}
