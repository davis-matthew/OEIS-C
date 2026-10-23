/*
 * A043414  Numbers whose base-7 representation contains exactly two 5's.
 * https://oeis.org/A043414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-7 representation contains exactly two 5's
   (OEIS A043414).  The function enumerates integers in order and counts
   the base‑7 digits that are 5 until the desired nth term is found. */

seq_t a(idx_t n)
{
    /* Count of terms found so far */
    idx_t found = 0;
    /* Current integer being examined */
    idx_t candidate = 0;

    while (1) {
        /* Count the number of 5's in the base‑7 representation of candidate */
        idx_t x = candidate;
        unsigned cnt = 0;
        while (x != 0 && cnt <= 2) {
            if ((x % 7) == 5) {
                ++cnt;
            }
            x /= 7;
        }

        /* If candidate has exactly two 5's, we have found a term */
        if (cnt == 2) {
            ++found;
            if (found == n) {
                /* Convert the 64‑bit candidate to the required seq_t type */
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
