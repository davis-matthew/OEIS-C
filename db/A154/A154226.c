/*
 * A154226  List of pairs: {(n*(n+1)/2)^2, (n!)^3}.
 * https://oeis.org/A154226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(2n) = (n*(n+1)/2)^2, a(2n+1) = (n!)^3
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Even indices: triangular number squared */
    if ((n & 1ULL) == 0) {
        seq_t k = (seq_t)(n / 2);
        seq_t t = k * (k + 1) / 2;
        return t * t;
    }

    /* Odd indices: factorial cubed with memoisation */
    idx_t k = (n - 1) / 2;          /* compute n! for n = k */

    /* Static cache of factorials: fact[i] = i! */
    static seq_t *fact = NULL;
    static idx_t cap   = 0;        /* allocated size of fact[] */
    static idx_t maxk  = 0;        /* largest k for which fact[k] is stored */

    if (!fact || maxk < k) {
        /* Initialise the cache if necessary */
        if (!fact) {
            cap = 1;
            fact = (seq_t *)malloc(sizeof(seq_t) * cap);
            fact[0] = 1;          /* 0! = 1 */
            maxk = 0;
        }

        /* Grow the array if it is too small */
        if (cap <= k) {
            size_t newcap = cap;
            while (newcap <= k) newcap *= 2;
            fact = (seq_t *)realloc(fact, sizeof(seq_t) * newcap);
            cap = newcap;
        }

        /* Compute new factorial values one by one */
        for (idx_t i = maxk + 1; i <= k; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        maxk = k;
    }

    seq_t f = fact[k];             /* k!  */
    return f * f * f;              /* (k!)^3 */
}
