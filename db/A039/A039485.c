/*
 * A039485  Numbers whose base-11 representation has the same nonzero number
 *          of 6's and 9's.
 * https://oeis.org/A039485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer whose base‑11 representation contains
   equal – and non‑zero – numbers of digits 6 and 9. */

seq_t a(idx_t n)
{
    /* Count found terms and iterate through positive integers. */
    idx_t  found = 0;
    seq_t  val   = 0;      /* current integer being tested */

    for (;; ++val) {
        seq_t temp   = val;
        unsigned c6 = 0, c9 = 0;

        /* Convert val to base 11 and count digits 6 and 9. */
        while (temp > 0) {
            unsigned d = (unsigned)(temp % 11);
            if (d == 6) ++c6;
            else if (d == 9) ++c9;
            temp /= 11;
        }

        /* Accept if both counts are equal and non‑zero. */
        if (c6 > 0 && c6 == c9) {
            ++found;
            if (found == n)
                return val;
        }
    }
}
