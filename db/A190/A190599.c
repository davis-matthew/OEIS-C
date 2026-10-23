/*
 * A190599  Maximal digit in base-11 expansion of n.
 * https://oeis.org/A190599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal digit in base‑11 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t max = 0;
    while (n > 0) {
        idx_t d = n % 11;
        if ((seq_t)d > max) {
            max = (seq_t)d;
        }
        n /= 11;
    }
    return max;
}
