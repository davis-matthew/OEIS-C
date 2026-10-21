/*
 * A054518  Emirpnons (nonprimes whose reversal is a different nonprime).
 * https://oeis.org/A054518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Emirpnons (nonprimes whose reversal is a different nonprime)
   Computed by brute‑force enumeration.  */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is the first Emirpnons number. */
    if (n == 0) return 0;          /* undefined for n==0, but harmless */

    idx_t count = 0;               /* how many Emirpnons found so far   */
    idx_t num   = 0;               /* current integer to test           */

    while (count < n) {
        ++num;                     /* advance to next integer           */
        if (!is_prime(num)) {      /* nonprime candidate                */
            idx_t revnum = rev(num);
            if (revnum != num && !is_prime(revnum)) {
                ++count;          /* a valid Emirpnons found           */
            }
        }
    }

    return (seq_t)num;             /* return the nth Emirpnons.          */
}
