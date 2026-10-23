/*
 * A039287  Numbers whose base-5 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The function enumerates numbers whose base‑5 representation
 *            contains the same non‑zero number of 1's and 2's. It finds the
 *            n‑th such number by a simple brute‑force search
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_equal_ones_twos(seq_t x)
{
    unsigned ones = 0, twos = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 5);
        if (d == 1)      ++ones;
        else if (d == 2) ++twos;
        x /= 5;
    }
    return ones == twos && ones > 0;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  There is no cache because the sequence can be
       generated quickly for n up to about 10,000 using a simple
       brute‑force search. */
    if (n == 0) return 0;           /* undefined index */

    idx_t  count = 0;
    seq_t  val   = 0;                /* start from 0 */

    while (count < n) {
        /* Increment val to the next integer. */
        ++val;
        if (has_equal_ones_twos(val)) {
            ++count;
        }
    }
    return val;
}
