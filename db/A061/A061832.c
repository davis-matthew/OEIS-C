/*
 * A061832  Multiples of 11 having only even digits.
 * https://oeis.org/A061832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate multiples of 11 and test that all digits are even.
   This yields the sequence of OEIS A061832. */

#include <stdbool.h>
#include "../utils/all_even_digits.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 22, a(3) = 44, ... */
    idx_t count = 0;          /* how many terms found so far */
    idx_t k = 0;              /* multiplier for 11 (k = 0 gives 0) */
    seq_t val;                /* current candidate value */

    if (n == 0) {             /* offset starts at 1; return 0 for safety */
        return 0;
    }

    for (;; ++k) {
        val = ((seq_t)k) * 11;              /* candidate multiple of 11 */
        if (all_even_digits(val)) {         /* all digits even? */
            ++count;
            if (count == n) {
                return val;                 /* found the n-th term */
            }
        }
    }
}
