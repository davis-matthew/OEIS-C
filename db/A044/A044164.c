/*
 * A044164  Numbers k such that string 3,5 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that base‑7 representation of k
 *            contains the substring "35", but base‑7 representation of k-1
 *            does not. (brute‑force search)
 * Range:     u8 n <= 6, u16 n <= 1445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has35
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has35.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 safely */

    seq_t k = 0;           /* candidate integer */
    idx_t found = 0;       /* how many suitable numbers found */
    bool prev_has = false; /* did k-1 contain "35"? */

    while (found < n)
    {
        ++k;                      /* next integer to test */
        bool cur_has = has35(k);  /* does k contain "35"? */

        if (cur_has && !prev_has) {
            ++found;
            if (found == n)
                return k;
        }
        prev_has = cur_has;
    }

    /* unreachable, kept for completeness */
    return (seq_t)0;
}
