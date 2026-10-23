/*
 * A101742  a(n+1) = smallest number greater than a(n) having either more
 *          divisors than a(n) or exactly as many divisors as a(n-1).
 * https://oeis.org/A101742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Not a closed form. Brute‑force search for the next term. a(1) =
 *            1. a(n+1) = smallest integer > a(n) having more divisors than
 *            a(n) or having exactly as many divisors as a(n‑1)
 * Range:     u8 n <= 37, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/d.h"   /* d(seq_t n) returns τ(n) */

static seq_t divisor_count(seq_t n) { return d(n); }

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static seq_t *taux = NULL;
    static idx_t sz = 0;
    static idx_t cap = 0;

    if (sz == 0) {          /* initialise with a(1) */
        cap = 16;
        vals = (seq_t *)malloc(sizeof(seq_t) * cap);
        taux = (seq_t *)malloc(sizeof(seq_t) * cap);
        vals[0] = (seq_t)1;
        taux[0] = divisor_count(vals[0]);   /* τ(1)=1 */
        sz = 1;
    }

    /* generate terms up to the requested index n */
    while (sz < n) {
        seq_t prev          = vals[sz - 1];
        seq_t prev_tau      = taux[sz - 1];
        seq_t prev_prev_tau = (sz >= 2) ? taux[sz - 2] : (seq_t)0;

        seq_t cand = prev + (seq_t)1;
        seq_t cand_tau;

        /* find the next suitable integer */
        while (1) {
            cand_tau = divisor_count(cand);
            if (cand_tau > prev_tau || (sz >= 2 && cand_tau == prev_prev_tau))
                break;
            cand += (seq_t)1;
        }

        if (sz == cap) {             /* grow storage if needed */
            cap <<= 1;
            vals = (seq_t *)realloc(vals, sizeof(seq_t) * cap);
            taux = (seq_t *)realloc(taux, sizeof(seq_t) * cap);
        }

        vals[sz] = cand;
        taux[sz] = cand_tau;
        ++sz;
    }

    return vals[n - 1];
}
