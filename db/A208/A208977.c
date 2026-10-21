/*
 * A208977  Self-convolution square-root of A005810, where A005810(n) =
 *          binomial(4*n,n).
 * https://oeis.org/A208977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, for n ≥ 2 :
 *            81*(n-1)*n*(2*n-3)*(3*n-2)*(3*n-1)*a(n) =
 *            24*(n-1)*(1152*n^4-4608*n^3+6698*n^2-4180*n+915)*a(n-1) -
 *            16*(2*n-1)*(8*n-15)*(8*n-13)*(8*n-11)*(8*n-9)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle trivial cases directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_upto = 0;        /* greatest n already computed */

    /* initialise cache for the first two terms */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cached_upto = 1;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        unsigned __int128 ni = (unsigned __int128)i;

        /* LHS coefficient */
        unsigned __int128 L =
            81 * (ni - 1) * ni *
            (2 * ni - 3) *
            (3 * ni - 2) *
            (3 * ni - 1);

        /* polynomial part for RHS term1 */
        unsigned __int128 poly =
            1152 * ni * ni * ni * ni
            - 4608 * ni * ni * ni
            + 6698 * ni * ni
            - 4180 * ni
            + 915;

        /* RHS = term1 - term2 */
        unsigned __int128 term1 =
            24 * (ni - 1) * poly * (unsigned __int128)cache[i - 1];
        unsigned __int128 term2 =
            16 *
            (2 * ni - 1) *
            (8 * ni - 15) *
            (8 * ni - 13) *
            (8 * ni - 11) *
            (8 * ni - 9) *
            (unsigned __int128)cache[i - 2];

        unsigned __int128 rhs = term1 - term2;
        unsigned __int128 val = rhs / L;

        cache[i] = (seq_t)val;
        cached_upto = i;
    }
    return cache[n];
}
