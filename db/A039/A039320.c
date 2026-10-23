/*
 * A039320  Numbers whose base-7 representation has the same nonzero number of
 *          2's and 4's.
 * https://oeis.org/A039320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑7 representation
 *            contains an equal (and non‑zero) number of digit 2 and digit 4.
 *            The function checks integers in increasing order and counts
 *            those satisfying the condition
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Return the n-th term of the sequence (n >= 1).  */
seq_t a(idx_t n)
{
    idx_t  count = 0;          /* how many valid numbers we have seen */
    seq_t  x = 1;              /* current integer to test */

    while (true) {
        seq_t  t = x;          /* make a copy of x for digit extraction */
        int    c2 = 0, c4 = 0;

        /* Extract base‑7 digits of t and count 2's and 4's */
        while (t > 0) {
            int d = (int)(t % (seq_t)7);   /* current digit (0..6) */
            if (d == 2) ++c2;
            else if (d == 4) ++c4;
            t = t / (seq_t)7;
        }

        /* If the count of 2's equals count of 4's and is non‑zero,
           update the running count and possibly return the result. */
        if (c2 == c4 && c2 > 0) {
            ++count;
            if (count == n)
                return x;
        }

        ++x;                  /* test next integer */
    }
}
