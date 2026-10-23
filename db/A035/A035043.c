/*
 * A035043  Replace any decimal digit '1' with '2' and vice versa.
 * https://oeis.org/A035043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal n with digits 1 and 2 swapped
 * Range:     u8 n <= 155, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t multiplier = 1;
    idx_t x = n;
    while (x > 0) {
        unsigned digit = x % 10;
        unsigned mapped;
        if (digit == 1)
            mapped = 2;
        else if (digit == 2)
            mapped = 1;
        else
            mapped = digit;
        result += (seq_t)mapped * multiplier;
        multiplier *= 10;
        x /= 10;
    }
    return result;
}
