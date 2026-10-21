/*
 * A221335  Number of nX3 arrays of occupancy after each element stays put or
 *          moves to some king-move neighbor, with every occupancy equal to
 *          zero or two.
 * https://oeis.org/A221335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221335
 * Empirical recurrence (valid for even n ≥ 16):
 *   a(n) = 66*a(n−2) −1131*a(n−4) +6726*a(n−6) −18808*a(n−8)
 *          +27860*a(n−10) −19388*a(n−12) +3240*a(n−14)
 *  Odd terms are zero.
 * Base values (even indices ≤ 14) are taken from the OEIS b-file.
 * The function memoises results in a static array up to MAX_N. */

#include <stddef.h>
#include <stdint.h>

/* Adjust MAX_N if you need to support larger n.  For n ≤ 10000 this
 * value is comfortably large. */
#define MAX_N 20001

/* Cache for previously computed terms.
 * Defined static so that it is shared across calls and automatically
 * zero‑initialised for integer types or default‑constructed for
 * arbitrary‑precision types. */
static seq_t cache[MAX_N];
/* Keeps track of the largest n for which cache[n] has been calculated. */
static idx_t last_computed = 0;
/* Indicates whether the base values have been written into the cache. */
static int cache_initialised = 0;

/* Base values for even n up to 14.  Odd indices are zero. */
static void initialise_cache(void)
{
    if (cache_initialised)
        return;

    /* a(0) is implicitly zero. */
    cache[1]  = (seq_t)0;
    cache[2]  = (seq_t)20;
    cache[3]  = (seq_t)0;
    cache[4]  = (seq_t)790;
    cache[5]  = (seq_t)0;
    cache[6]  = (seq_t)34042;
    cache[7]  = (seq_t)0;
    cache[8]  = (seq_t)1470618;
    cache[9]  = (seq_t)0;
    cache[10] = (seq_t)63526554;
    cache[11] = (seq_t)0;
    cache[12] = (seq_t)2744129114;
    cache[13] = (seq_t)0;
    cache[14] = (seq_t)118536728562;
    cache_initialised = 1;
    last_computed = 14;
}

seq_t a(idx_t n)
{
    /* The function is indexed starting at 1 as per OEIS convention. */
    if (n < 1)
        return (seq_t)0; /* undefined, but return zero */

    if (n >= MAX_N) {
        /* If n exceeds the pre‑allocated array size, compute it
         * recursively on one shot.  This fallback is only a
         * safety‑net and should not be reached for the standard
         * test range (n ≤ 10000). */
        seq_t res = (seq_t)0;
        for (idx_t i = 1; i <= n; ++i) {
            res = a(i); /* recursion depth is at most linear; safe for small n */
        }
        return res;
    }

    initialise_cache();

    /* Extend precomputation up to n if necessary. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i & 1U) {                /* all odd indices are zero */
                cache[i] = (seq_t)0;
            } else {                     /* even indices */
                /* Safe to use a(i‑k) for any k ≤ 14 because
                 * a(0) through a(14) are already defined. */
                seq_t term =  (seq_t)66  * cache[i - 2]
                           - (seq_t)1131 * cache[i - 4]
                           + (seq_t)6726 * cache[i - 6]
                           - (seq_t)18808 * cache[i - 8]
                           + (seq_t)27860 * cache[i -10]
                           - (seq_t)19388 * cache[i -12]
                           + (seq_t)3240 * cache[i -14];
                cache[i] = term;
            }
        }
        last_computed = n;
    }

    return cache[n];
}
