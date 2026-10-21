/*
 * A054593  Number of disconnected labeled digraphs with n nodes.
 * https://oeis.org/A054593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(n-1)) - A003027(n) where A003027(n) is the number
 *            of connected labeled digraphs. A003027 satisfies A003027(1) = 1
 *            A003027(n) = 2^(n*(n-1)) - Σ_{k=1}^{n-1} binom(n-1,k-1) *
 *            A003027(k) * 2^((n-k)*(n-k-1)) (n ≥ 2) The disconnected count is
 *            simply the sum of the above terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* safety for misuse */

    /* --- static persistent storage ------------------------------------- */
    static size_t cap = 0;          /* allocated size (indices 0..cap-1)   */
    static seq_t *pow_cache  = NULL;/* 2^(m*(m-1))  for m = index          */
    static seq_t *connected = NULL; /* connected digraphs count              */
    static seq_t *disconnected = NULL; /* disconnected digraphs count       */
    static idx_t  highest = 0;      /* highest n that has been computed    */

    /* ++ ensure capacity for indices up to n ---------------------------- */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_pow   = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
        seq_t *new_conn  = (seq_t *)realloc(connected, new_cap * sizeof(seq_t));
        seq_t *new_dis   = (seq_t *)realloc(disconnected, new_cap * sizeof(seq_t));
        if (!new_pow || !new_conn || !new_dis) {
            #ifdef __STDC_NO_VLA__ /* if VLA not available, use error handling */
            /* allocation failure – return 0 for the test harness */
            #endif
            return (seq_t)0;
        }
        pow_cache     = new_pow;
        connected     = new_conn;
        disconnected = new_dis;
        /* initialize new elements to zero */
        for (size_t i = cap; i < new_cap; ++i) {
            pow_cache[i]     = (seq_t)0;
            connected[i]     = (seq_t)0;
            disconnected[i]  = (seq_t)0;
        }
        cap = new_cap;
    }

    /* ++ pre‑compute pow_cache for new indices --------------------------- */
    for (size_t m = (size_t)highest + 1; (idx_t)m <= n; ++m) {
        /*  2^(m*(m-1))  */
        pow_cache[m] = pow2( (idx_t)m * (m - 1) );
    }

    /* ++ compute connected and disconnected counts for fresh indices ---- */
    for (idx_t i = highest + 1; i <= n; ++i) {
        if (i == 1) {
            connected[1]      = (seq_t)1;
            disconnected[1]   = (seq_t)0;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t term = binom(i - 1, k - 1);
                term *= connected[k];
                term *= pow_cache[i - k];   /* 2^((i-k)*(i-k-1)) */
                sum += term;
            }
            connected[i]      = pow_cache[i] - sum;
            disconnected[i]   = sum;
        }
    }
    highest = n;
    return disconnected[n];
}
