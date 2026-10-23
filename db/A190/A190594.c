/*
 * A190594  Maximal digit in base-5 expansion of n.
 * https://oeis.org/A190594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = maximum digit in the base‑5 expansion of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t max_digit = 0;
    idx_t x = n;
    while (x > 0) {
        seq_t digit = x % 5;
        if (digit > max_digit)
            max_digit = digit;
        x /= 5;
    }
    return max_digit;
}
