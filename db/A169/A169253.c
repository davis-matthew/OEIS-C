/*
 * A169253  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46*a(n-1) + 46*a(n-2) + … + 46*a(n-27) – 1081*a(n-28),
 *            with a(0)=1, a(1)=a(2)=…=a(27)=2, a(28)=1, and a(n)=0 for n>28
 *            in the inhomogeneous term
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes and type definitions are assumed from the build environment.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*--- static memoization buffer ------------------------------------*/
    static seq_t *cache  = NULL;   /* array of computed terms          */
    static idx_t  cap    = 0;      /* allocated size (in elements)     */
    static idx_t  comp   = 0;      /* number of terms stored (0..comp‑1) */

    /*--- initialize cache on first call --------------------------------*/
    if (cache == NULL) {
        cap   = 32;                     /* start with a few slots          */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure: return zero */
    }

    /*--- grow buffer if needed ------------------------------------------*/
    if (n >= cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* re‑alloc failure                */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /*--- compute missing terms ------------------------------------------*/
    if (n >= comp) {
        /* small or big integer: choose an appropriate temporary type   */
        const bool small = (sizeof(seq_t) <= 8);  /* 8 bytes or less fits in 64-bit */

        for (idx_t i = comp; i <= n; ++i) {
            /* inhomogeneous part from the numerator polynomial */
            int base_int = 0;
            if (i == 0)                 base_int = 1;
            else if (i <= 27)           base_int = 2;
            else if (i == 28)           base_int = 1;
            else                        base_int = 0;   /* >28 */

            if (small) {
                __int128 val = (__int128)base_int;
                /* sum 46 * a[i-k] for k=1..27 (or up to i)                 */
                idx_t maxk = (i < 27) ? i : 27;
                for (idx_t k = 1; k <= maxk; ++k) {
                    val += (__int128)cache[i - k] * 46;
                }
                /* subtract 1081 * a[i-28] if i >= 28                       */
                if (i >= 28) {
                    val -= (__int128)cache[i - 28] * 1081;
                }
                cache[i] = (seq_t)val;  /* truncates if overflow              */
            } else {
                seq_t val = (seq_t)base_int;
                idx_t maxk = (i < 27) ? i : 27;
                for (idx_t k = 1; k <= maxk; ++k) {
                    val += cache[i - k] * (seq_t)46;
                }
                if (i >= 28) {
                    val -= cache[i - 28] * (seq_t)1081;
                }
                cache[i] = val;
            }
        }
        comp = n + 1;   /* all indices up to n are now stored               */
    }

    return cache[n];
}
