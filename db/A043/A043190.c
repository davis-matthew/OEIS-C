/*
 * A043190  Numbers k such that 1 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate integers k and find the nth number whose base‑9
   representation contains the digits 1 and 4 as an adjacent pair
   (either "14" or "41"), while the representation of k‑1 does not
   contain such a pair.  This implements OEIS A043190 as described. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* test whether base‑9 representation of x contains an adjacent
   1–4 or 4–1 pair */
static bool has14_base9(seq_t x)
{
    if (x < (seq_t)9) return false;            /* fewer than two digits */
    seq_t last = x % (seq_t)9;                 /* least significant digit */
    x     /= (seq_t)9;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)9;                /* next more significant digit */
        if ((d == (seq_t)1 && last == (seq_t)4) ||
            (d == (seq_t)4 && last == (seq_t)1))
            return true;
        last = d;
        x    /= (seq_t)9;
    }
    return false;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* behaviour for n==0 is irrelevant */

    idx_t   found = 0;                        /* number of valid terms found */
    seq_t   k     = (seq_t)1;                 /* current candidate integer */

    while (found < n) {
        if (has14_base9(k) && !has14_base9(k - (seq_t)1)) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
    return k;                                 /* never reached, keeps compiler happy */
}
