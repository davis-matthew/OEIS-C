/*
 * A039399  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 7's.
 * https://oeis.org/A039399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-10 representation has the same nonzero
   number of 0's and 7's.  The implementation searches integers
   sequentially and returns the n-th such number (1‑based index). */

seq_t a(idx_t n)
{
    idx_t   found = 0;
    seq_t   val   = 1;
    for (;;) {
        /* Count 0's and 7's in the decimal representation of 'val'. */
        seq_t  t = val;
        unsigned zeros = 0, sevens = 0;
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d == 0)    zeros++;
            else if (d == 7) sevens++;
            t /= 10;
        }
        if (zeros == sevens && zeros > 0) {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }
}
