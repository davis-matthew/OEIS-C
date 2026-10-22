/*
 * A004516  Generalized nim sum n + n in base 6.
 * https://oeis.org/A004516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + n in base 6, computed digit-wise mod 6 with no
 *            carries
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t multiplier = 1;
    seq_t n_seq = (seq_t)n;
    
    while (n_seq > 0) {
        seq_t digit = n_seq % 6;
        seq_t doubled = (digit + digit) % 6;
        result += doubled * multiplier;
        multiplier *= 6;
        n_seq /= 6;
    }
    
    return result;
}
