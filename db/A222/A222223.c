/*
 * A222223  In the number n, replace all (decimal) digits '1' with '6' and
 *          vice versa.
 * https://oeis.org/A222223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal-digit-wise swap of 1 ↔ 6 in n
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t result = 0;
    seq_t mult = 1;
    while (x > 0) {
        seq_t digit = x % 10;
        seq_t mapped;
        if (digit == 1)
            mapped = 6;
        else if (digit == 6)
            mapped = 1;
        else
            mapped = digit;
        result += mapped * mult;
        mult *= 10;
        x /= 10;
    }
    return result;
}
