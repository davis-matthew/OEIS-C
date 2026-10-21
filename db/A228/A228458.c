/*
 * A228458  Number of arrays of maxima of three adjacent elements of some
 *          length n+2 0..5 array.
 * https://oeis.org/A228458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 14*a(n-5) + 49*a(n-6)
 *            + 8*a(n-7) + 15*a(n-8) + 54*a(n-9) + 21*a(n-10) + 20*a(n-11) +
 *            26*a(n-12) + 10*a(n-13) + 5*a(n-14) + 4*a(n-15) + a(n-16) (valid
 *            for n ≥ 17; initial values a(1)=6,…,a(16)=873867641)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


#if defined(__cplusplus)
#  include <type_traits>
#endif

/* --------------------------- helper ------------------------------------ */
static inline seq_t mul_small(seq_t v, unsigned int mul)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value)
        return (seq_t)((__uint128_t) v * mul);
    else
        return v * (seq_t) mul;
#else
    /* C: use 128‑bit intermediate for all integral types */
    return (seq_t)((__uint128_t) v * mul);
#endif
}

/* --------------------------- constants --------------------------------- */
static const seq_t INITIAL[16] = {
    6ULL, 36ULL, 161ULL, 581ULL, 1847ULL, 5615ULL,
    17487ULL, 57303ULL, 194064ULL, 659418ULL,
    2212695ULL, 7329603ULL, 24138450ULL, 79571792ULL,
    263302273ULL, 873867641ULL
};

/* --------------------------- cache ----------------------------------- */
static seq_t *cache      = NULL;   /* dynamic array, 1‑based indexing (cache[0] unused) */
static size_t cache_cap  = 0;      /* allocated capacity (bytes/entries) */
static idx_t  cache_len  = 0;      /* highest index that is valid */

/* --------------------------- ensure_cache ----------------------------- */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;          /* already known */

    /* enlarge array if necessary */
    if ((idx_t)(cache_cap) < (idx_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing initial terms up to min(n,16) */
    size_t fill_to = (n < 16) ? (size_t)n : 16;
    if (cache_len < fill_to) {
        for (size_t j = cache_len + 1; j <= fill_to; ++j)
            cache[j] = INITIAL[j - 1];
        cache_len = (idx_t)fill_to;
    }

    /* compute recurrence for indices >16, if needed */
    if (n > 16) {
        idx_t start = (cache_len >= 16) ? (cache_len + 1) : 17;  /* cache_len must be 16 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 0;
            val += mul_small(cache[i - 1], 6);
            val -= mul_small(cache[i - 2], 15);
            val += mul_small(cache[i - 3], 20);
            val -= mul_small(cache[i - 5], 14);
            val += mul_small(cache[i - 6], 49);
            val += mul_small(cache[i - 7], 8);
            val += mul_small(cache[i - 8], 15);
            val += mul_small(cache[i - 9], 54);
            val += mul_small(cache[i -10], 21);
            val += mul_small(cache[i -11], 20);
            val += mul_small(cache[i -12], 26);
            val += mul_small(cache[i -13], 10);
            val += mul_small(cache[i -14], 5);
            val += mul_small(cache[i -15], 4);
            val += cache[i - 16];
            cache[i] = val;
        }
        cache_len = n;   /* all terms up to n are now valid */
    }
}

/* --------------------------- sequence --------------------------------- */
seq_t a(idx_t n)           /* n >= 1 */
{
    ensure_cache(n);
    return cache[n];
}
