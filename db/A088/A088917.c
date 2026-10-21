/*
 * A088917  Central Delannoy numbers (mod 3); Characteristic function for
 *          Cantor set.
 * https://oeis.org/A088917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the base‑3 representation of n contains no digit 1,
 *            otherwise 0. (Characteristic function of the Cantor middle‑third
 *            set)
 * Range:     u8 n <= 9097, u16 n <= 9097, u32 n <= 9097, u64 n <= 9097,
 *            u128 n <= 9097, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while (n != 0) {
        if ((n % 3) == 1)
            return (seq_t)0;
        n /= 3;
    }
    return (seq_t)1;
}
