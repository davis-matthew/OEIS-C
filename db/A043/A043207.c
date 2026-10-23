/*
 * A043207  Numbers k such that 4 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adj46
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A043207 –  Numbers k such that the base‑9 representation of k
 *  contains an adjacent pair 4,6 (in either order) but the base‑9
 *  representation of k‑1 does not.  The function returns the
 *  n‑th term (1‑based) by scanning forward from 1, without caching.
 *  This works for any auxiliary build where seq_t is an integer type
 *  (fixed‑width or arbitrary‑precision).
 *
 *  It uses only integer arithmetic, the operators + - * / %, and
 *  comparisons.  No dynamic allocation or external utilities are
 *  required.  The algorithm is O(n·log₉(n)) which is easily below
 *  the 2s limit for n up to 10 000.  The function is order‑independent
 *  – subsequent calls with any n will recompute the required range.
 */
#include <stdbool.h>
#include "../utils/has_adj46.h"

/*
 *  Return the n‑th element of A043207 (1‑based).  For n = 0 (out of
 *  the offset) the function returns 0 – the behaviour is unspecified
 *  beyond that.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, undefined for n<1 */

    seq_t count = 0;                         /* number of found terms */
    seq_t k = 0;                             /* current integer being tested */

    while (count < n) {
        ++k;
        if (has_adj46(k) && !has_adj46(k - 1))
            ++count;
    }
    return k;
}
