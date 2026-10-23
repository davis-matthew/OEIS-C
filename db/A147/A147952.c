/*
 * A147952  a(0) = 0, a(1) = a(2) = 1, and for n >= 3, a(n) = a(a(n-2)) +
 *          r(n), where r(n) = a(a(floor(n/3))) when n == 0 or 1 (mod 3) and =
 *          a(n - a(floor(n/3))) when n == 2 (mod 3).
 * https://oeis.org/A147952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = a(2) = 1, a(n) = a(a(n-2)) + r(n), n ≥ 3 r(n) =
 *            a(a(floor(n/3))) if n ≡ 0 (mod 3) a(a(floor((n-1)/3))) if n ≡ 1
 *            (mod 3) a(n - a(floor((n-2)/3))) if n ≡ 2 (mod 3)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the base cases directly */
    if (n == 0u) return (seq_t)0;
    if (n == 1u || n == 2u) return (seq_t)1;

    /* Allocate an array large enough for a[0]..a[n] */
    seq_t *cache = (seq_t *)malloc((n + 1u) * sizeof(seq_t));
    if (!cache) return (seq_t)0; /* allocation failure – return 0 */

    /* initialise the first three values */
    cache[0u] = (seq_t)0;
    cache[1u] = cache[2u] = (seq_t)1;

    /* compute a[i] for i = 3 .. n */
    for (idx_t i = 3u; i <= n; ++i) {
        /* compute the r(i) term */
        seq_t r;
        if ((i % 3u) == 0u) {
            /* i ≡ 0 (mod 3) : r = a(a(floor(i/3))) */
            seq_t inner = cache[i / 3u];
            r = cache[(idx_t)inner];
        } else if ((i % 3u) == 1u) {
            /* i ≡ 1 (mod 3) : r = a(a(floor((i-1)/3))) */
            seq_t inner = cache[(i - 1u) / 3u];
            r = cache[(idx_t)inner];
        } else {
            /* i ≡ 2 (mod 3) : r = a(i - a(floor((i-2)/3))) */
            seq_t inner = cache[(i - 2u) / 3u];
            idx_t idx2 = i - (idx_t)inner;
            r = cache[idx2];
        }

        /* compute the first part a(a(i-2)) */
        seq_t a_i_minus_2 = cache[i - 2u];
        seq_t inner2 = cache[(idx_t)a_i_minus_2];

        cache[i] = inner2 + r;
    }

    seq_t result = cache[n];
    free(cache);
    return result;
}
