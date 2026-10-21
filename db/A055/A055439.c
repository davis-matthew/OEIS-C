/*
 * A055439  Distribution of first digit of mantissa following Benford's law,
 *          using Sainte-Laguë method to minimize chi-squared statistic.
 * https://oeis.org/A055439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm:  produce the Benford first‑digit distribution using the
 *  Sainte‑Laguë method.  At each step n, the next digit d is chosen to
 *  maximise  P(d)/(2·c_d+1)  where P(d)=log10(1+1/d) and c_d is the
 *  current count of digit d.  The sequence is memoised so any call
 *  receives the correctly pre‑computed term.  Only integer arithmetic
 *  is used; the P(d) values are stored as large‐fixed‑point integers
 *  scaled by 10^12.  All operations fit in 64 bits for the limits
 *  used (n≈10 000).  */
#include <stdlib.h>
#include <stdint.h>

/* 1‑based indices:  Pscaled[d] = round( log10(1+1/d) * 10^12 )  */
static const uint64_t Pscaled[10] = {
    0ULL,
    301029995664ULL,
    176091259056ULL,
    124938736608ULL,
    96910013008ULL,
    79181246047ULL,
    66946789631ULL,
    57991946978ULL,
    51152522447ULL,
    45757490561ULL
};

/*  local memoisation   */
static uint16_t  counts[10] = {0};            /* current counts of 1…9   */
static seq_t    *cache    = NULL;            /* cached a(1)…a(cachelen)*/
static size_t   cachecap = 0;                /* allocated capacity       */
static size_t   cachelen = 0;                /* filled elements          */

/*  make sure cache can hold n elements  */
static void
ensure_cache_capacity(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cachecap) return;
    size_t newcap = cachecap? cachecap * 2 : 16;
    while (need > newcap) newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) { /* out of memory – abort */
        abort();
    }
    cache = tmp;
    cachecap = newcap;
}

/*  a(n):  return nth term of A055439  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined, but harmless */

    ensure_cache_capacity(n);

    /*  compute terms up to n if not already present   */
    for (idx_t i = cachelen + 1; i <= n; ++i) {
        /*  choose digit with maximum Pscaled[d]/(2*counts[d]+1)  */
        int best = 1;
        uint64_t best_num = Pscaled[1];
        uint64_t best_den = 2ULL * counts[1] + 1ULL;

        for (int d = 2; d <= 9; ++d) {
            uint64_t num = Pscaled[d];
            uint64_t den = 2ULL * counts[d] + 1ULL;
            uint64_t left  = num * best_den;
            uint64_t right = best_num * den;
            if (left > right || (left == right && d < best)) {
                best = d;
                best_num = num;
                best_den = den;
            }
        }
        cache[i - 1] = (seq_t)best;   /* store the selected digit   */
        ++counts[best];
    }
    cachelen = (size_t)n;
    return cache[n - 1];
}
