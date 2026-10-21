/*
 * A052874  E.g.f.: -x/(-1+x)*(exp(-x/(-1+x))-1).
 * https://oeis.org/A052874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052874
   Formula: a(n) = n! * ( LaguerreL(n-1, -1) - 1 )  (n >= 1)
   Recurrence (n >= 0):
      -(n^4+6n^3+11n^2+6n)*a(n)
      +(3n^3+18n^2+33n+18)*a(n+1)
      -(3n^2+14n+15)*a(n+2)
      +(n+2)*a(n+3) = 0
   Initial values: a(0)=0, a(1)=0, a(2)=2
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static idx_t  cache_len = 0;     /* number of allocated slots */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 2;

    /* grow cache if needed */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed – caller will deal */
        cache = new_cache;
        cache_len = new_len;
    }

    /* initialise first three entries if not yet done */
    if (cache_len > 0) {
        if (cache_len > 0) cache[0] = 0;
        if (cache_len > 1) cache[1] = 0;
        if (cache_len > 2) cache[2] = 2;
    }

    /* compute missing values up to n */
    for (idx_t i = 3; i <= n; ++i) {
        /* recurrence variable m = i-3 */
        idx_t m   = i - 3;
        /* coefficients fit in 64‑bit for n <= 10000 */
        unsigned long long m64 = (unsigned long long)m;
        unsigned long long A = m64 * m64 * m64 * m64
                           + 6ULL * m64 * m64 * m64
                           + 11ULL * m64 * m64
                           + 6ULL * m64;
        unsigned long long B = 3ULL * m64 * m64 * m64
                           + 18ULL * m64 * m64
                           + 33ULL * m64
                           + 18ULL;
        unsigned long long C = 3ULL * m64 * m64
                           + 14ULL * m64
                           + 15ULL;

        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit type: use 128‑bit intermediates to avoid overflow */
            unsigned __int128 term1 = (unsigned __int128)A * (unsigned __int128)cache[m];
            unsigned __int128 term3 = (unsigned __int128)C * (unsigned __int128)cache[m+2];
            unsigned __int128 term2 = (unsigned __int128)B * (unsigned __int128)cache[m+1];
            unsigned __int128 num   = term1 + term3 - term2;   /* non‑negative by recurrence */
            cache[i] = (seq_t)(num / (m64 + 2ULL));
        } else {
            /* arbitrary‑precision or wider type: normal arithmetic works */
            seq_t t1 = cache[m] * (seq_t)A;
            seq_t t3 = cache[m+2] * (seq_t)C;
            seq_t sum = t1 + t3;
            seq_t t2 = cache[m+1] * (seq_t)B;
            sum -= t2;           /* division result is integer */
            cache[i] = sum / (m + 2);
        }
    }

    return cache[n];
}
