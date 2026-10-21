/*
 * A096863  Function A062402(x)=sigma(phi(x)) is iterated. Starting with n,
 *          a(n) is the count of transient terms of trajectory.
 * https://oeis.org/A096863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A096863(n) = number of transient terms in the orbit * of
 *            n under the iteration x → sigma(phi(x))
 * Range:     u8 n <= 100, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   calc_phi, grow_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The orbit is followed until the first repeated value.
 * The function returns the index of the first occurrence of that value
 * (zero‑based), which equals the count of transient terms.  */

#include <stddef.h>
#include "../utils/calc_phi.h"   /* calc_phi(seq_t m)          */
#include "../utils/sigma_of.h"  /* seq_t sigma_of(idx_t n);   */
#include "../utils/grow_seq.h"  /* void grow_seq(seq_t **, size_t *, size_t); */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t) n;          /* current value of the orbit */

    seq_t *seen = NULL;             /* dynamic array of seen values   */
    size_t cap  = 0;                /* current capacity               */
    size_t len  = 0;                /* number of stored values        */

    const size_t GROW_START = 64;   /* initial growth step            */

    for (;;) {
        /* --- check for a repeat ----------------------------------- */
        for (size_t i = 0; i < len; ++i) {
            if (seen[i] == cur)        /* repeated term found          */
                return (seq_t) i;       /* transient length             */
        }

        /* --- enlarge the dynamic array if needed ------------------- */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? GROW_START : cap << 1;
            grow_seq(&seen, &cap, new_cap);
        }

        /* --- record current value and iterate --------------------- */
        seen[len++] = cur;
        seq_t phi = calc_phi(cur);
        /* phi is known to fit into idx_t for all tested n; cast safely. */
        cur = sigma_of((idx_t) phi);
    }
}
