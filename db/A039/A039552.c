/*
 * A039552  Numbers whose base-12 representation has the same nonzero number
 *          of 7's and 11's.
 * https://oeis.org/A039552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes the n-th element of OEIS sequence A039552:
   numbers whose base‑12 representation contains the same non‑zero
   number of digits 7 and 11.  The function performs a simple
   search: it tests successive integers until the requested
   index is reached.  The search works for any seq_t type supplied
   by the build system.
*/

seq_t a(idx_t n)
{
    /* The index is 1‑based; just loop until the n‑th match is found */
    const seq_t BASE   = (seq_t)12;
    const seq_t DIG7   = (seq_t)7;
    const seq_t DIG11  = (seq_t)11;

    seq_t current = (seq_t)1;           /* candidate number */
    idx_t found   = (idx_t)0;           /* number of matches found */

    while (1) {
        /* Count 7’s and 11’s in the base‑12 expansion of current */
        seq_t x = current;
        unsigned cnt7  = 0;
        unsigned cnt11 = 0;

        while (x > (seq_t)0) {
            seq_t digit = x % BASE;
            if (digit == DIG7)   cnt7++;
            else if (digit == DIG11) cnt11++;
            x = x / BASE;
        }

        /* A match has one or more 7’s and 11’s and they are equal */
        if (cnt7 > 0 && cnt7 == cnt11) {
            found++;
            if (found == n)
                return current;
        }

        current++;
    }
}
