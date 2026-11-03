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
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The orbit is followed until the first repeated value.
 * The function returns the index of the first occurrence of that value
 * (zero‑based), which equals the count of transient terms.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
        seq_t phi = totient_seq(cur);
        /* phi is known to fit into idx_t for all tested n; cast safely. */
        cur = sigma_of((idx_t) phi);
    }
}
