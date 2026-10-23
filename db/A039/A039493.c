/*
 * A039493  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 1's.
 * https://oeis.org/A039493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates integers in increasing order and returns the nth integer
   whose base‑12 representation contains an equal non‑zero number of 0’s
   and 1’s. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many valid numbers found so far   */
    seq_t x = 0;              /* current candidate number              */

    while (count < n) {
        ++x;                  /* move to next integer                */
        seq_t tmp = x;
        unsigned zeros = 0;
        unsigned ones  = 0;

        /* Count digits in base 12 */
        while (tmp) {
            seq_t digit = tmp % 12;
            if (digit == 0) ++zeros;
            else if (digit == 1) ++ones;
            tmp /= 12;
        }

        if (zeros == ones && zeros > 0)
            ++count;
    }

    return x;
}
