/*
 * A102671  Number of digits >= 3 in decimal representation of n.
 * https://oeis.org/A102671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n that are >=3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t m = n;               // n fits in uint64_t (idx_t is uint64_t)
    if (m == 0) return count;     // 0 has no digits >=3
    while (m > 0) {
        uint64_t d = m % 10;      // get least significant digit
        if (d >= 3) count++;      // count if digit is at least 3
        m /= 10;
    }
    return count;
}
