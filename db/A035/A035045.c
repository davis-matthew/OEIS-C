/*
 * A035045  Inverse binomial transform of A002054.
 * https://oeis.org/A035045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies (n+3)*(3*n-1)*a(n) = (6*n^2+19*n+7)*a(n-1) +
 *            3*(n-1)*(3*n+2)*a(n-2) with a(0)=1, a(1)=4. The implementation
 *            uses memoisation and 128‑bit intermediates for the built‑in
 *            numeric types, while the C++ version uses arbitrary‑precision
 *            arithmetic (vector storage)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

/* --- C implementation ---------------------------------------------------- */
#ifndef __cplusplus
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated array size */
    static idx_t computed = 0;    /* number of terms already computed */

    /* grow cache if needed */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* initialise base cases once */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }
    if (computed == 1) {
        cache[1] = (seq_t)4;
        computed = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        unsigned __int128 sn = i;                /* promote index to 128‑bit */
        unsigned __int128 term1 = (unsigned __int128)(
            (6 * sn * sn) + (19 * sn) + 7) * (unsigned __int128)cache[i - 1];
        unsigned __int128 term2 = (unsigned __int128)(
            3 * (sn - 1) * (3 * sn + 2)) * (unsigned __int128)cache[i - 2];
        unsigned __int128 num   = term1 + term2;
        unsigned __int128 den   = (sn + 3) * (3 * sn - 1);
        cache[i] = (seq_t)(num / den);           /* division is exact */
    }

    computed = n + 1;    /* all terms up to n computed */
    return cache[n];
}
#endif /* !__cplusplus */

/* --- C++ implementation --------------------------------------------------- */
#ifdef __cplusplus
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;

    if (n >= cache.size()) {
        idx_t prev = cache.size();            /* first index to compute */
        cache.resize(n + 1);                   /* enlarge to required size */

        for (idx_t i = prev; i <= n; ++i) {
            seq_t term1 = ((seq_t)6 * i * i + (seq_t)19 * i + (seq_t)7) * cache[i - 1];
            seq_t term2 = (seq_t)3 * (i - 1) * (3 * i + 2) * cache[i - 2];
            seq_t den   = (i + 3) * (3 * i - 1);
            cache[i] = (term1 + term2) / den;   /* exact division          */
        }
    }

    return cache[n];
}
#endif /* __cplusplus */
