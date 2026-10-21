/*
 * A072726  Numerator of the rationals >= 1 whose continued fractions consist
 *          of only even terms, in ascending order by the sum of the continued
 *          fraction terms and descending by lowest order continued fraction
 *          terms to highest.
 * https://oeis.org/A072726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072726 – Numerator of rationals ≥ 1 whose continued fractions consist of
   only even terms, ordered by ascending sum of the continued fraction terms
   and by descending lowest order terms to highest.
   Recurrence:
   a(0) = 1
   a(2^k) = 2·(k+1)
   a(2^k + 2^j + m) = 2·(k-j)·a(2^j + m) + a(m) (when 2^k > 2^j > m ≥ 0)   */

#include <stdint.h>
#include <stdlib.h>

/* helper to find floor(log2(n)) for n>0 */
static inline idx_t floor_log2_uint64(idx_t n) {
    /* use builtin if available, otherwise fallback */
#ifdef __GNUC__
    return 63 - __builtin_clzll(n);
#else
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
#endif
}

/* memoisation cache */
static seq_t *cache      = NULL;
static size_t cache_size = 0;

/* recursive evaluation of a(n) */
static seq_t a_rec(idx_t n) {
    if (n == 0) return (seq_t)1;

    if (n < cache_size && cache[n] != 0) return cache[n];

    seq_t result;

    /* power of two */
    if ((n & (n - 1)) == 0) {
        idx_t k = floor_log2_uint64(n);
        result = (seq_t)2 * ((seq_t)(k + 1));
    } else {
        /* decompose n = 2^k + 2^j + m, with 2^k > 2^j > m >= 0 */
        idx_t k   = floor_log2_uint64(n);
        idx_t rest= n - (1ull << k);
        idx_t j   = floor_log2_uint64(rest);
        idx_t m   = rest - (1ull << j);

        seq_t part1 = a_rec((1ull << j) + m);
        seq_t part2 = a_rec(m);

        result = (seq_t)2 * ((seq_t)(k - j)) * part1 + part2;
    }

    /* enlarge cache if needed */
    if (n >= cache_size) {
        size_t new_size = n + 1;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        if (new_size == 0) new_size = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);               /* out of memory */
        if (new_size > cache_size) {
            /* initialise new cells to zero (except 0th) */
            for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    cache[n] = result;
    return result;
}

/* required external function */
seq_t a(idx_t n) {
    return a_rec(n);
}
