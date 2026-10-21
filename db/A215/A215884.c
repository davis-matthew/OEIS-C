/*
 * A215884  Written in base 5, n ends in a(n) consecutive nonzero digits.
 * https://oeis.org/A215884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of consecutive trailing nonzero digits in the
 *            base‑5 representation of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 0) {
        if (n % 5 == 0)
            break;
        ++count;
        n /= 5;
    }
    return count;
}
