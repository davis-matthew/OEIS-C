/*
 * A051809  Numbers with nonzero multiplicative digital root 7.
 * https://oeis.org/A051809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   contains_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A051809
   Numbers with nonzero multiplicative digital root 7.
   For n ≥ 1 the function returns the n‑th number in this sequence.
   Implementation: simple backward‑search for each n independently.
   It skips numbers that do not contain the digit 7, because a root of 7
   can only appear if the number itself contains a 7.  Each candidate
   is tested by repeatedly multiplying its non‑zero decimal digits until
   a single‑digit result is obtained; if that result is 7 the candidate
   is counted as a term of the sequence.  No caching or dynamic memory
   is required, so the code compiles for both fixed‑width integer
   builds and for builds that use an arbitrary‑precision class for seq_t.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains_seven.h"

/* --------------------------------------------------------------------- */

/* Helper: product of the non‑zero decimal digits of a number.          */
static idx_t prod_nonzero_digits(idx_t n)
{
    idx_t prod = 1;
    while (n) {
        idx_t d = n % 10;
        if (d != 0) prod *= d;
        n /= 10;
    }
    return prod;
}

/* Helper: multiplicative digital root of a number (ignoring zeros).    */
static idx_t md_root(idx_t n)
{
    while (n >= 10) n = prod_nonzero_digits(n);
    return n;
}

/* --------------------------------------------------------------------- */
/* Main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* behaviour below offset ignored   */

    idx_t found = 0;
    idx_t cand   = 1;                          /* first candidate to examine       */

    while (1) {
        /* advance to next candidate that contains a 7          */
        do { ++cand; } while (!contains_seven(cand));

        if (md_root(cand) == 7) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}
