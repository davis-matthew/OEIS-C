/*
 * A004774  Numbers n whose binary expansion does not end in 001.
 * https://oeis.org/A004774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-2)/7⌋ + (((n-2) mod 7) or 7 if 0); a(1) = 0, a(2) =
 *            1
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2)
        return (seq_t)(n - 1);
    
    idx_t x = n - 2;
    idx_t q = x / 7;
    idx_t r = x % 7;
    
    return (seq_t)(8 * q + (r == 0 ? 0 : r + 1));
}
