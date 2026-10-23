/*
 * A025913  Expansion of 1/((1-x^7)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A025913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of partitions of n into parts 7, 9, and 11
 *            Computed by counting all non‑negative integer solutions of 7*x +
 *            9*y + 11*z = n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t zmax = n / 11;                     /* maximum number of 11‑parts */
    for (idx_t z = 0; z <= zmax; ++z) {
        idx_t rem_z = n - 11 * z;             /* remaining sum after choosing z */
        idx_t ymax = rem_z / 9;               /* maximum number of 9‑parts */
        for (idx_t y = 0; y <= ymax; ++y) {
            idx_t rem_y = rem_z - 9 * y;     /* remaining sum after choosing y */
            if ((rem_y % 7) == 0)            /* if divisible by 7, a valid triple */
                ++count;
        }
    }
    return count;
}
