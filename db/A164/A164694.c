/*
 * A164694  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47*(a(n-1)+…+a(n-6)) - 1128*a(n-7) for n ≥ 8 a(0)=1,
 *            a(1)=49, a(2)=2352, a(3)=112896, a(4)=5419008, a(5)=260112384,
 *            a(6)=12485394432. a(7) = 47*(a(1)+…+a(6)) - 1128*a(0) + 1 =
 *            599298931560
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  size  = 0;            /* number of already computed entries  */
    static size_t cap   = 0;            /* allocated capacity (in entries)      */

    /* ---------- first use: initialise cache with the first 8 values ---------- */
    if (size == 0) {
        cap = 16;                                 /* initial capacity              */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed              */

        cache[0] = (seq_t)1ull;
        cache[1] = (seq_t)49ull;
        cache[2] = (seq_t)2352ull;
        cache[3] = (seq_t)112896ull;
        cache[4] = (seq_t)5419008ull;
        cache[5] = (seq_t)260112384ull;
        cache[6] = (seq_t)12485394432ull;

        /* compute a(7) using the special numerator‑contribution rule   */
        seq_t sum6 = cache[1] + cache[2] + cache[3] +
                      cache[4] + cache[5] + cache[6];
        cache[7] = (seq_t)47 * sum6 - (seq_t)1128 * cache[0] + 1;

        size = 8;                                  /* a[0..7] now known              */
    }

    /* ---------- grow the cache if the requested index is out of bounds ---------- */
    if ((idx_t)(n + 1) > cap) {
        size_t newcap = cap;
        while ((idx_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* reallocation failed           */
        cache = tmp;
        cap   = newcap;
    }

    /* ---------- return a known value if already cached ---------- */
    if (n < size) return cache[n];

    /* ---------- compute a(n) for n ≥ size (which is ≥ 8) ---------- */
    /* pre‑compute the sum of the last six terms that are already known */
    seq_t sum = cache[size-1] + cache[size-2] + cache[size-3] +
                cache[size-4] + cache[size-5] + cache[size-6];

    for (idx_t i = size; i <= n; ++i) {
        /* a(i) = 47 * sum_of_prev_six - 1128 * a(i-7) */
        seq_t val = (seq_t)47 * sum - (seq_t)1128 * cache[i-7];
        cache[i] = val;

        /* Update the running sum for the next step */
        if (i + 1 <= n) {
            sum += cache[i];          /* add new term */
            sum -= cache[i-6];        /* remove term that falls out of the 6‑term window */
        }
    }

    size = n + 1;          /* update the number of cached entries */
    return cache[n];
}
