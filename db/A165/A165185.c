/*
 * A165185  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=5, a(2)=20, a(3)=80, a(4)=320, a(5)=1280,
   a(6)=5120, a(7)=20480, a(8)=81920,
   a(9)=3*(a(8)+a(7)+…+a(1)) − 6*a(0)+1,
   for n ≥ 10: a(n)=3*(a(n−1)+a(n−2)+…+a(n−8)) − 6*a(n−9). */

#ifdef __cplusplus
#include <vector>
#endif

/* The first nine terms.  The cast is safe for every allowed seq_t.  */
static const seq_t init[9] = {
    (seq_t)1ULL,   /* a(0) */
    (seq_t)5ULL,   /* a(1) */
    (seq_t)20ULL,  /* a(2) */
    (seq_t)80ULL,  /* a(3) */
    (seq_t)320ULL, /* a(4) */
    (seq_t)1280ULL,/* a(5) */
    (seq_t)5120ULL,/* a(6) */
    (seq_t)20480ULL,/* a(7) */
    (seq_t)81920ULL  /* a(8) */
};

#ifndef __cplusplus
/* C implementation ---------------------------------------------------- */
#include <stdlib.h>

/* Cache for the sequence values. */
static seq_t *cache = NULL;           /* array of values */
static idx_t cache_capacity = 0;      /* number of allocated slots */
static idx_t cache_size     = 0;      /* current number of stored values */

static void ensure_capacity(idx_t n)
{
    if (cache_capacity <= n) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (cache_size <= n) {
        /* Grow the cache if necessary. */
        ensure_capacity(n);

        /* Compute values from the first missing entry up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i <= 8) {                /* initial values */
                val = init[i];
            } else if (i == 9) {         /* special case for n=9 */
                seq_t sum = 0;
                for (int j = 1; j <= 8; ++j) sum += cache[i - j];
                val = (seq_t)3 * sum - (seq_t)6 * cache[0] + (seq_t)1;
            } else {                     /* recurrence for n≥10 */
                seq_t sum = 0;
                for (int j = 1; j <= 8; ++j) sum += cache[i - j];
                val = (seq_t)3 * sum - (seq_t)6 * cache[i - 9];
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
#else
/* C++ implementation --------------------------------------------------- */
seq_t a(idx_t n)
{
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(9);
        cache.push_back(init[0]); cache.push_back(init[1]);
        cache.push_back(init[2]); cache.push_back(init[3]);
        cache.push_back(init[4]); cache.push_back(init[5]);
        cache.push_back(init[6]); cache.push_back(init[7]);
        cache.push_back(init[8]);
    }

    while (cache.size() <= n) {
        idx_t i = cache.size();            /* next index to compute */
        seq_t val;
        if (i <= 8) {
            val = init[i];
        } else if (i == 9) {
            seq_t sum = 0;
            for (int j = 1; j <= 8; ++j) sum += cache[i - j];
            val = (seq_t)3 * sum - (seq_t)6 * cache[0] + (seq_t)1;
        } else {
            seq_t sum = 0;
            for (int j = 1; j <= 8; ++j) sum += cache[i - j];
            val = (seq_t)3 * sum - (seq_t)6 * cache[i - 9];
        }
        cache.push_back(val);
    }

    return cache[n];
}
#endif
