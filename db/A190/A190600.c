/*
 * A190600  Maximal digit in base-12 expansion of n.
 * https://oeis.org/A190600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum digit in the base‑12 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t max_digit = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        seq_t d = (seq_t)(tmp % 12);
        if (d > max_digit)
            max_digit = d;
        tmp /= 12;
    }
    return max_digit;
}
