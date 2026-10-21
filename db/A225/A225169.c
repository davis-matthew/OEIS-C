/*
 * A225169  Denominators of the sequence s(n) of the sum resp. product of
 *          fractions f(n) defined recursively by f(1) = 10/1; f(n+1) is
 *          chosen so that the sum and the product of the first n terms of the
 *          sequence are equal.
 * https://oeis.org/A225169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1.  Maintain the reduced fraction
 * s(n)=p/q.  For n>=1 let
 *   f(n+1)=p/(p‑q)   (chosen so sum and product of the first n terms are equal),
 *   s(n+1)=p²/(q·(p‑q)).
 *
 * After reduction we have new p,q.  The denominator a(n+1) is the reduced
 * denominator of s(n+1), i.e. q after the reduction.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache of already computed denominators; a(1) stored at index 0 */
    static seq_t *denoms     = NULL;     /* array of denominators */
    static size_t denoms_cap = 0;        /* capacity of the array   */
    static idx_t denoms_len  = 0;        /* number of values stored */

    /* current fraction s(denoms_len):  p/q   (denoms_len=0 means 10/1) */
    static seq_t current_p = 10; /* numerator of s(denoms_len + 1) when denoms_len==0 */
    static seq_t current_q = 1;  /* denominator of s(denoms_len + 1) */

    if (n == 0) return 0; /* a(0) is undefined, return 0 for safety */

    /* Ensure we have enough space for n denominators (0‑based) */
    if ((size_t)n > denoms_cap) {
        size_t new_cap = denoms_cap ? denoms_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(denoms, denoms_cap, new_cap);
        if (!tmp) abort();          /* allocation failed; terminate */
        denoms     = tmp;
        denoms_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = denoms_len; i < n; ++i) {
        /* a(i+1) is the current denominator */
        denoms[i] = current_q;

        /* Compute next fraction s(i+2) */
        seq_t next_p = current_p * current_p;
        seq_t next_q = current_q * (current_p - current_q);
        seq_t g      = gcd(next_p, next_q);
        next_p /= g;
        next_q /= g;
        current_p = next_p;
        current_q = next_q;
    }

    denoms_len = n;
    return denoms[n - 1];
}
