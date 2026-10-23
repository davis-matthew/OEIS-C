/*
 * A025553  a(n) = (Sum_{i=0..n-1} 1/b(i)) * LCM{b(i): i=0..n-1}, where b(i) =
 *          C(i,floor(i/2)).
 * https://oeis.org/A025553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{i=0}^{n-1} 1/b(i)) * LCM{b(i)} where b(i) =
 *            C(i,⌊i/2⌋)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/binom.h"

static seq_t *a_cache      = NULL;   /* cache of already computed terms */
static size_t a_cache_cap  = 0;       /* capacity of the cache           */
static idx_t   a_cache_n   = 0;       /* number of terms stored (=cur_n) */
static seq_t   L_cur;                 /* current LCM of processed b(i)    */
static seq_t   S_cur;                 /* current sum  Σ L_cur / b(i)     */

seq_t a(idx_t n)
{
    if (n < 1) {                 /* out of range – undefined, return 0 */
        return (seq_t)0;
    }

    /* Initialise state at first use.  */
    if (a_cache_n == 0) {
        L_cur = (seq_t)1;
        S_cur = (seq_t)0;
    }

    /* Expand cache if necessary.  */
    if ((size_t)n + 1 > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure – fatal */
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Compute missing terms up to n.  */
    for (idx_t i = a_cache_n; i < n; ++i) {
        idx_t k = i >> 1;                     /* floor(i/2) */
        seq_t bi = binom(i, k);               /* b(i) = C(i, floor(i/2)) */

        seq_t g = gcd(L_cur, bi);             /* gcd of current LCM and bi */
        seq_t m = bi / g;                     /* multiplier to reach new LCM */

        L_cur = L_cur * m;                    /* new LCM */
        seq_t L_div_b = L_cur / bi;           /* LCM / bi (exact) */

        S_cur = S_cur * m + L_div_b;          /* update sum */

        a_cache[i + 1] = S_cur;               /* store result a(i+1) */
    }

    a_cache_n = n;                            /* all terms up to n are known */
    return a_cache[n];
}
