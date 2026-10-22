/*
 * A005428  a(n) = ceiling((1 + sum of preceding terms) / 2) starting with
 *          a(0) = 1.
 * https://oeis.org/A005428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((3*a(n-1) + p(n-1)) / 2), p(n) = (a(n-1) + p(n-1))
 *            mod 2, a(0) = 1, p(0) = 0, where p(n) is the parity of
 *            Sum_{k=0..n-1} a(k); equivalent to a(n) = ceiling((1 +
 *            Sum_{k=0..n-1} a(k)) / 2). Evaluated as a(n-1) + floor((a(n-1) +
 *            p(n-1)) / 2) so no intermediate exceeds a(n)
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 55, u64 n <= 109,
 *            u128 n <= 219, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static unsigned char *par = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        par = (unsigned char *)malloc(1);
        if (cache == NULL || par == NULL) return 0;
        cache[0] = 1;
        par[0] = 0;
        cache_len = 1;
    }

    if (n < cache_len) return cache[n];

    seq_t *nc = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (nc == NULL) return 0;
    cache = nc;
    unsigned char *np = (unsigned char *)realloc(par, (size_t)(n + 1));
    if (np == NULL) return 0;
    par = np;

    for (idx_t i = cache_len; i <= n; i++) {
        seq_t x = cache[i - 1];
        unsigned p = par[i - 1];
        seq_t half = (x + p) / 2;
        cache[i] = x + half;
        par[i] = (unsigned char)(((unsigned)(x & 1) + p) & 1);
    }

    cache_len = n + 1;
    return cache[n];
}
