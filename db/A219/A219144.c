/*
 * A219144  Unchanging value maps: number of 3Xn binary arrays indicating the
 *          locations of corresponding elements unequal to no horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 3Xn array.
 * https://oeis.org/A219144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -8*a(n-3) +17*a(n-4) +17*a(n-5) -29*a(n-6)
 *            -13*a(n-7) +9*a(n-8) -31*a(n-9) -26*a(n-10) +30*a(n-11)
 *            -3*a(n-12) +6*a(n-13) +17*a(n-14) -3*a(n-15) +3*a(n-16)
 *            -2*a(n-17) (for n>18) Initial values: a(1)=1, a(2)=18, a(3)=78,
 *            a(4)=281, a(5)=1072, a(6)=4033, a(7)=15255, a(8)=57963,
 *            a(9)=2...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation to allow out‑of‑order calls. */

#include <stdlib.h>   /* malloc, realloc, free, abort */

static seq_t *cache = NULL;          /* cache[i] holds a(i+1) */
static idx_t cache_len = 0;          /* number of allocated cache entries */

/* The first 18 terms of the sequence */
static const seq_t init[18] = {
    (seq_t)1ULL, (seq_t)18ULL, (seq_t)78ULL, (seq_t)281ULL,
    (seq_t)1072ULL, (seq_t)4033ULL, (seq_t)15255ULL, (seq_t)57963ULL,
    (seq_t)220068ULL, (seq_t)835820ULL, (seq_t)3172817ULL,
    (seq_t)12044346ULL, (seq_t)45722019ULL, (seq_t)173574485ULL,
    (seq_t)658947323ULL, (seq_t)2501581220ULL, (seq_t)9496787730ULL,
    (seq_t)36052730049ULL
};

/* Recurrence coefficients for k = 1..17 (coeff[0] unused) */
static const int coeff[18] = {
    0,            /* unused */
    4,            /* a(n-1)  */
    0,            /* a(n-2)  */
    -8,           /* a(n-3)  */
    17,           /* a(n-4)  */
    17,           /* a(n-5)  */
    -29,          /* a(n-6)  */
    -13,          /* a(n-7)  */
    9,            /* a(n-8)  */
    -31,          /* a(n-9)  */
    -26,          /* a(n-10) */
    30,           /* a(n-11) */
    -3,           /* a(n-12) */
    6,            /* a(n-13) */
    17,           /* a(n-14) */
    -3,           /* a(n-15) */
    3,            /* a(n-16) */
    -2            /* a(n-17) */
};

/* Ensure that cache contains at least n entries and that the
   initial 18 terms are present.  */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    size_t new_cap = (size_t)n;
    if (new_cap < 64) new_cap = 64;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    /* zero newly allocated part if any */
    for (size_t i = cache_len; i < new_cap; ++i) new_cache[i] = 0;

    /* initialise the first 18 terms */
    size_t to_copy = new_cap < 18 ? new_cap : 18;
    for (size_t i = 0; i < to_copy; ++i) new_cache[i] = init[i];

    cache = new_cache;
    cache_len = (idx_t)new_cap;
}

/* Compute a(n) for n ≥ 1.  The function is memoised so that
   any order of calls is supported. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    ensure_cache(n);

    if (n <= 18) return cache[n - 1];

    /* compute from successive terms up to n */
    for (idx_t i = 19; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= 17; ++k) {
            int c = coeff[k];
            if (c == 0) continue;
            seq_t prod = (seq_t)(c > 0 ? c : -c) *
                         cache[(i - (idx_t)k) - 1];
            if (c > 0) pos += prod;
            else       neg += prod;
        }
        /* pos >= neg by definition of recurrence */
        cache[i - 1] = pos - neg;
    }

    return cache[n - 1];
}
