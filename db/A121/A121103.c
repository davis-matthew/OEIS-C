/*
 * A121103  a(1)=1; a(n) = the reversal of (a(n-1) + sfd(a(n-1))), where
 *          sfd(n)= sum of factorials of the digits of n.
 * https://oeis.org/A121103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=1; a(n)=reverse(a(n-1)+sfd(a(n-1)))
 * where sfd(m)=sum of factorials of the decimal digits of m.
 * The reversal is done in base‑10.
 */

#include <stdlib.h>

/* factorials of digits 0–9, casted to seq_t to avoid overflow */
static const seq_t fact[10] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,  (seq_t)24,
    (seq_t)120, (seq_t)720, (seq_t)5040, (seq_t)40320, (seq_t)362880
};

/* reverse the decimal representation of x */
static seq_t reverse_dec(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* cache of already computed terms */
static seq_t *a_cache   = NULL;   /* array indexed from 1 */
static idx_t  cache_len = 0;     /* largest n computed so far */
static idx_t  cache_cap = 0;     /* allocated capacity */

/* sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* not used by OEIS offset 1 */
    }

    /* initialise or grow the cache if necessary */
    if (cache_len == 0) {
        cache_cap = n;
        a_cache   = (seq_t *)malloc((size_t)(cache_cap + 1) * sizeof(seq_t));
        a_cache[1] = (seq_t)1;          /* a(1) = 1 */
        cache_len  = 1;
    } else if (cache_len < n) {
        if (cache_cap < n) {
            cache_cap = n;
            a_cache = (seq_t *)realloc(a_cache,
                                       (size_t)(cache_cap + 1) * sizeof(seq_t));
        }
    }

    /* if we already have the required term, just return it */
    if (cache_len >= n) {
        return a_cache[n];
    }

    /* start from the last computed term */
    seq_t cur = a_cache[cache_len];

    /* compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sfd = 0;
        seq_t t   = cur;
        while (t > 0) {
            int d = (int)(t % 10);
            sfd += fact[d];
            t   /= 10;
        }
        seq_t sum = cur + sfd;
        cur = reverse_dec(sum);
        a_cache[i] = cur;
    }

    cache_len = n;
    return cur;
}
