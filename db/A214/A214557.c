/*
 * A214557  Fixed points of the Kaprekar map A151949, of the form
 *          8{p+1}6{p+1}4{p+1}3{q}2{p}19{p+1}7{p+1}6{q}5{p+1}3{p+1}1{p}2,
 *          where d{n} is digit d repeated n times, p, q >= 0.
 * https://oeis.org/A214557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 7, u128 n <= 34,
 *            big any n
 * Helpers:   append_times, build_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fixed points of the Kaprekar map A151949 as described in OEIS A214557.
   The terms are constructed from the formula
      8{p+1}6{p+1}4{p+1}3{q}2{p}19{p+1}7{p+1}6{q}5{p+1}3{p+1}1{p}2
   with p,q ≥ 0.  They are ordered first by increasing total length
      L = 9 + 9p + 2q,
   then by increasing p.  The routine below computes the n‑th term on
   demand without any dynamic allocation, so it works for all
   validated build configurations. */

/* Standard headers that are valid for both C and C++. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/append_times.h"
#include "../utils/build_term.h"

/* Main entry point: returns the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence defined for n ≥ 1 */

    idx_t remaining = n;                 /* how many terms still to skip */
    idx_t L = 9;                          /* start with minimal length */

    for (;; ++L) {
        /* For this length, p ranges from 0 to floor((L-9)/9). */
        idx_t p_max = (L > 9) ? (L - 9) / 9 : 0;
        idx_t p_start = ((L - 9) & 1ULL) ? 1 : 0;   /* p must match parity of (L-9) */
        idx_t count = 0;

        if (p_start <= p_max) {
            /* Number of p values with step 2 */
            count = (p_max - p_start) / 2 + 1;
        }

        if (remaining > count) {
            remaining -= count;         /* skip this block */
            continue;
        }

        /* Found the correct block. Compute p and q. */
        idx_t i = remaining - 1;         /* zero‑based index among the p's */
        idx_t p = p_start + 2 * i;
        idx_t q = (L - 9 - 9 * p) / 2;

        return build_term(p, q);
    }
}
