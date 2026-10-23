/*
 * A194251  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=3-sqrt(8).
 * https://oeis.org/A194251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194251:
   a(n) = floor( Σ_{k=1..n} frac( r·k ) ),   r = 3 – sqrt(8),
   where [x] = floor(x) and (x) = frac(x). */

/* Computes a(n) for any n ≥ 1.  Uses memoised prefix sums of floor(k·sqrt(8)). */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Outside defined range – return 0 to avoid UB */
        return (seq_t)0;
    }

    /* ---------------------------------------------
       1.  Memoised prefix sum of floor(k·sqrt(8))
       --------------------------------------------- */
    static seq_t *pref = NULL;      /* pref[k] = Σ_{i=1..k} floor(i·sqrt(8)) */
    static idx_t  pref_cap = 0;     /* capacity of pref array            */
    static idx_t  pref_len = 0;     /* number of entries currently set   */

    /* Allocate or grow the array so that pref[n] is available */
    if (pref == NULL) {
        pref_cap = 16;
        pref = (seq_t *)malloc(pref_cap * sizeof(seq_t));
        if (!pref) abort();           /* allocation failed */
        pref[0] = (seq_t)0;
        pref_len = 1;
    } else if (pref_cap <= n) {
        /* double until we have room for index n */
        while (pref_cap <= n) pref_cap <<= 1;
        pref = (seq_t *)realloc(pref, pref_cap * sizeof(seq_t));
        if (!pref) abort();           /* allocation failed */
    }

    /* Extend prefix sum from pref_len to n */
    for (idx_t k = pref_len; k <= n; ++k) {
        /* floor(k·sqrt(8)) = floor_sqrt(8·k²) */
        seq_t kk = (seq_t)k;
        seq_t val = floor_sqrt( (seq_t)8 * kk * kk );
        pref[k] = pref[k - 1] + val;
    }
    pref_len = n + 1;

    /* ---------------------------------------------
       2.  Compute a(n) using the closed form:
           a(n) = Σ floor(k·sqrt(8)) + n – 1 – floor_sqrt(8·T²)
       where T = n(n+1)/2
       --------------------------------------------- */
    /* T = n(n+1)/2 as seq_t */
    seq_t T = (seq_t)n * ( (seq_t)n + (seq_t)1 ) / (seq_t)2;

    /* Compute floor_sqrt(8·T²) */
    seq_t prod = T * T;
    prod *= (seq_t)8;
    seq_t fsqrt = floor_sqrt(prod);

    seq_t result = pref[n] + (seq_t)(n - 1) - fsqrt;
    return result;
}
