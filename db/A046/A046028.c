/*
 * A046028  Largest multiple prime factor of the n-th nonsquarefree number
 *          (A013929).
 * https://oeis.org/A046028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest prime factor of the n-th nonsquarefree number
 *            that appears with exponent ≥ 2 (the repeated prime factor of
 *            that number)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_squarefree.h"

static idx_t *spf = NULL;            /* smallest prime factor array   */
static idx_t  spf_cap = 0;           /* current limit of spf array    */

/* ensure that spf has at least 'needed' entries (0 … needed)   */
static void expand_spf(idx_t needed)
{
    if (needed <= spf_cap) return;
    if (spf) free(spf);
    build_spf(needed, &spf);        /* spf[0] … spf[needed]          */
    spf_cap = needed;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – not used          */

    /* Rough upper bound for the nth nonsquarefree number.
       Density of nonsquarefree numbers ≈ 0.39207, so n/0.39 ≈ 2.556n.
       Using 3 n + 100 (plus a safety margin) is more than enough.    */
    idx_t limit = n * 3 + 100;
    expand_spf(limit);

    idx_t count = 0;
    idx_t candidate = 4;            /* first nonsquarefree number      */
    for (;; ++candidate) {
        /* test whether candidate is nonsquarefree */
        if (!is_squarefree((seq_t)candidate)) {
            ++count;
            if (count == n) break;
        }
    }

    /* candidate now holds the nth nonsquarefree number.  Find
       its largest repeated prime factor using the spf array. */
    idx_t temp = candidate;
    seq_t maxp = 0;

    while (temp > 1) {
        idx_t p = spf[temp];
        if (p == 0) p = temp;       /* prime number      */
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp >= 2 && (seq_t)p > maxp) {
            maxp = (seq_t)p;
        }
    }
    return maxp;
}
