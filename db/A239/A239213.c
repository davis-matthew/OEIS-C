/*
 * A239213  Squares that are divisible by each of their nonzero digits.
 * https://oeis.org/A239213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate squares that are divisible by each of their non‑zero digits. */

#include <stdbool.h>
#include <stddef.h>

/* Return the n‑th term of OEIS sequence A239213.  */
seq_t a(idx_t n)
{
    if (n < 1)    /* Undefined for n < 1, but return 0 for safety. */
        return 0;

    idx_t found = 0;   /* How many qualifying squares have been seen.  */
    idx_t i = 1;       /* Current integer whose square is tested.     */

    while (found < n) {
        seq_t sq = (seq_t)i * (seq_t)i;      /* Square of i.                */

        /* Test that every non‑zero decimal digit divides the square. */
        seq_t tmp = sq;
        bool ok = true;
        while (tmp > 0) {
            int d = (int)(tmp % (seq_t)10);  /* Current digit.            */
            if (d != 0) {
                if (sq % (seq_t)d != 0) {
                    ok = false;
                    break;
                }
            }
            tmp /= (seq_t)10;
        }

        if (ok) {
            found++;
            if (found == n)
                return sq;                    /* Found the required term. */
        }
        i++;                                 /* Next integer to test.   */
    }

    /* Control never reaches here, but satisfy the compiler. */
    return 0;
}
