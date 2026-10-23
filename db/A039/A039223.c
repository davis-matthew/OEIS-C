/*
 * A039223  Numbers whose base-12 representation has the same number of 1's
 *          and 5's.
 * https://oeis.org/A039223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039223: Numbers whose base‑12 representation has
   the same number of 1's and 5's. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t value = 0;

    while (found < n) {
        /* Count digits 1 and 5 in base 12 */
        idx_t c1 = 0, c5 = 0;
        seq_t temp = value;
        while (temp > 0) {
            seq_t d = temp % ((seq_t)12);
            if (d == ((seq_t)1))
                ++c1;
            else if (d == ((seq_t)5))
                ++c5;
            temp /= ((seq_t)12);
        }
        if (c1 == c5) {
            ++found;
            if (found == n)
                return value;
        }
        ++value;
    }
    /* Should never reach here */
    return value;
}
