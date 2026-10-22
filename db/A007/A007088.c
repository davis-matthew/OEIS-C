/*
 * A007088  The binary numbers (or binary words, or binary vectors, or binary
 *          expansion of n): numbers written in base 2.
 * https://oeis.org/A007088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = Sum_{k>=0} (bit_k of n) * 10^k (each binary digit becomes
 *            a decimal digit)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 9833,
 *            u128 n <= 9833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power_of_10 = 1;
    
    while (n > 0) {
        if (n & 1) {
            result += power_of_10;
        }
        n >>= 1;
        power_of_10 *= 10;
    }
    
    return result;
}
