/*
 * A043727  Numbers whose number of runs in the base-2 representation is
 *          congruent to 3 mod 4.
 * https://oeis.org/A043727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose binary representation has
 *            a number of runs congruent to 3 modulo 4
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    idx_t  seen = 0;          /* number of qualifying numbers found so far */
    seq_t  x    = 1;          /* current integer to test */

    while (1) {
        /* Count runs in the binary representation of x. */
        seq_t y      = x;
        int    runs  = 0;
        seq_t  prev  = 0;
        int    first = 1;          /* flag for the first bit seen */

        while (y != 0) {
            seq_t cur = y % 2;     /* least‑significant bit */
            if (first) {
                runs  = 1;
                prev  = cur;
                first = 0;
            } else if (cur != prev) {
                runs++;
                prev = cur;
            }
            y = y / 2;
        }

        if ((runs & 3) == 3) {     /* runs ≡ 3 (mod 4) */
            seen++;
            if (seen == n)
                return x;
        }

        x++;                       /* advance to next integer */
    }
}
