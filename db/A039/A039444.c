/*
 * A039444  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 7's.
 * https://oeis.org/A039444
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

/* Sequence A039444: numbers whose base-11 representation has the same
   nonzero number of 0's and 7's. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a value has an equal (and non‑zero) number of
   digit 0 and digit 7 in its base‑11 representation. */
static inline bool good(seq_t val)
{
    int zeros = 0, sevens = 0;
    while (val > 0) {
        int d = (int)(val % 11);          /* digit in base 11 */
        if (d == 0)   ++zeros;
        else if (d == 7) ++sevens;
        val /= 11;
    }
    return (zeros == sevens) && (zeros > 0);
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset < 1 */

    idx_t found = 0;              /* number of terms found so far */
    idx_t cand   = 1;              /* current candidate value */

    for (;; ++cand) {
        if (good((seq_t)cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
