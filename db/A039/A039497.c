/*
 * A039497  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 5's.
 * https://oeis.org/A039497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑12 representation has the same nonzero number
 *            of 0's and 5's. The routine finds the n‑th such number by a
 *            simple brute‑force search
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x has the same strictly positive number of digits
   0 and 5 in its base‑12 representation. */
static bool matches(seq_t x)
{
    int zeros = 0, fives = 0;
    while (x > 0) {
        int d = (int)(x % 12);
        if (d == 0)  zeros++;
        else if (d == 5) fives++;
        x /= 12;
    }
    return (zeros > 0 && fives > 0 && zeros == fives);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for non‑positive indices */

    seq_t count = 0;
    seq_t candidate = 0;

    while (1) {
        if (matches(candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;               /* move to next integer */
    }
}
