/*
 * A187547  L(n)H(n+1), product of the central Lah number L(n) and the
 *          harmonic number H(n).
 * https://oeis.org/A187547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n) * H(n+1) Where L(n) are the central Lah numbers.
 *            Recurrence: (n+3)(n+2)(n+1)a(n+2) - 4(2n+3)^2(2n+5)(n+1)a(n+1) +
 *            16(2n+3)^2(2n+1)^2(n+2)a(n) - 144δ(n,0) = 0 Initial values:
 *            a(0)=1, a(1)=3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Return previously computed value if available */
    if (n < cache_len)
        return cache[n];

    /* Initialise cache with first two terms if empty */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out of memory – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Grow cache to required size */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* For i>=2 we use the recurrence with n = i-2 */
        if (i < 2) {                 /* safety: should not happen */
            cache[i] = (seq_t)(i == 0 ? 1 : 3);
            continue;
        }
        unsigned __int128 m = (unsigned __int128)(i - 2);
        unsigned __int128 A = 2 * m + 3;
        unsigned __int128 term1 = 4 * A * A * (2 * m + 5) * (m + 1)
                                 * (unsigned __int128)cache[i - 1];
        unsigned __int128 term2 = 16 * A * A * (2 * m + 1) * (2 * m + 1)
                                 * (m + 2) * (unsigned __int128)cache[i - 2];
        unsigned __int128 delta = (m == 0) ? 144 : 0;
        unsigned __int128 numer = term1 - term2 + delta;
        unsigned __int128 denom = (m + 3) * (m + 2) * (m + 1);

        /* Integer division – the recurrence guarantees divisibility */
        cache[i] = (seq_t)(numer / denom);
    }
    cache_len = n + 1;
    return cache[n];
}
