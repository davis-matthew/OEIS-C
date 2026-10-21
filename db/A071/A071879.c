/*
 * A071879  G.f. satisfies: A(x) = 1 + x*A(x) + x^3*A(x)^3.
 * https://oeis.org/A071879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071879
 *  Sequence definition (OEIS):
 *      a(n) = Sum_{k=0..⌊n/3⌋} binomial(n,3k) * binomial(3k,k) / (2k+1)
 *  Strict recurrence for n ≥ 3:
 *      2*n*(2*n+3)*a(n)
 *      + 2*(1-6*n^2)*a(n-1)
 *      + 6*(2*n-1)*(n-1)*a(n-2)
 *      - 31*(n-1)*(n-2)*a(n-3) = 0
 *  Initial values: a(0)=a(1)=a(2)=1.
 *  The implementation memoises all computed values and uses the
 *  recurrence above.  For 64‑bit integer types an intermediate
 *  unsigned __int128 is employed to avoid overflow; for arbitrary‑
 *  precision types the arithmetic is performed directly with seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation storage */
static seq_t *cache      = NULL;   /* array of values */
static size_t cap        = 0;      /* allocated capacity */
static idx_t  computed   = 0;      /* number of values already stored */

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Return cached value immediately if available */
    if (n < computed)
        return cache[n];

    /* Ensure the array is large enough for n */
    idx_t needed = n + (idx_t)1;
    if ((size_t)needed > cap) {
        if (cache == NULL)
            cache = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
        cap = (size_t)needed;
    }

    /* Compute all missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i <= 2) {                     /* a(0)=a(1)=a(2)=1 */
            cache[i] = (seq_t)1;
            continue;
        }

        /* For 64‑bit seq_t use unsigned __int128 for intermediate values */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 ni   = (unsigned __int128)i;
            unsigned __int128 denom =
                (unsigned __int128)2 * ni * ((unsigned __int128)2 * ni + 3);

            unsigned __int128 termA =
                ((unsigned __int128)12 * ni * ni - 2) * (unsigned __int128)cache[i-1];

            unsigned __int128 termB =
                (unsigned __int128)6 *
                ((unsigned __int128)2 * ni - 1) *
                (ni - 1) *
                (unsigned __int128)cache[i-2];

            unsigned __int128 termC =
                (unsigned __int128)31 *
                (ni - 1) *
                (ni - 2) *
                (unsigned __int128)cache[i-3];

            unsigned __int128 num = termA + termC - termB;
            cache[i] = (seq_t)(num / denom);
        } else {
            /* For arbitrary‑precision types we can use seq_t directly */
            seq_t ni   = (seq_t)i;

            seq_t denom = ((seq_t)2 * ni) * ((seq_t)2 * ni + (seq_t)3);

            seq_t termA = ( ((seq_t)12 * ni * ni) - (seq_t)2 ) * cache[i-1];

            seq_t termB = ((seq_t)6 *
                           ((seq_t)2 * ni - (seq_t)1) *
                           (ni - (seq_t)1)) * cache[i-2];

            seq_t termC = ((seq_t)31 *
                           (ni - (seq_t)1) *
                           (ni - (seq_t)2)) * cache[i-3];

            seq_t num = termA + termC - termB;
            cache[i] = num / denom;
        }
    }

    computed = needed;
    return cache[n];
}
