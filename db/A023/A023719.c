/*
 * A023719  Numbers with exactly two 3's in their base 4 expansion.
 * https://oeis.org/A023719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers whose base‑4 representation contains exactly two digits 3. */

seq_t a(idx_t n)
{
    seq_t v = 0;          /* current candidate value */
    idx_t found = 0;      /* how many valid numbers have been seen */

    for (;; ++v) {
        seq_t tmp = v;
        idx_t cnt3 = 0;

        /* Count base‑4 digits equal to 3. */
        while (tmp > 0) {
            if ((tmp & 3) == 3)
                ++cnt3;
            tmp >>= 2;
        }

        if (cnt3 == 2) {
            ++found;
            if (found == n)
                return v;
        }
    }
}
