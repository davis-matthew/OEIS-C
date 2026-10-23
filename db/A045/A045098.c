/*
 * A045098  Numbers whose base-4 representation contains exactly two 1's and
 *          three 2's.
 * https://oeis.org/A045098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑4 representation contains
 *            exactly two 1's and three 2's
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 0;
    const seq_t mask = 3; /* mask for extracting a base‑4 digit */

    while (1) {
        ++x;
        seq_t tmp = x;
        unsigned c1 = 0, c2 = 0;

        while (tmp > 0) {
            unsigned d = (unsigned)(tmp & mask);
            if (d == 1) ++c1;
            else if (d == 2) ++c2;
            tmp >>= 2;   /* same as tmp /= 4 for unsigned integers */
        }

        if (c1 == 2 && c2 == 3) {
            ++found;
            if (found == n) return x;
        }
    }
}
