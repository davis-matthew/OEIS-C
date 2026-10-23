/*
 * A170333  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function of A170333:
 *
 * a(0) = 1
 * For n ≥ 1,
 *    sum = a(n-1) + a(n-2) + … + a(n-41)   (ignore terms with negative index)
 *    a(n) = 34 * sum
 *           + b(n)
 *           - (595 * a(n-42))  if n ≥ 42
 *
 * where b(n) =
 *      1  for n = 0 or 42
 *      2  for 1 ≤ n ≤ 41
 *      0  otherwise
 *
 * The sequence is memoised so calls in arbitrary order are efficient.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use a std::vector to store the cache. */
    static std::vector<seq_t> cache;

    if (cache.empty()) {
        cache.reserve(1);
        cache.push_back((seq_t)1);   /* a(0) = 1 */
    }

    /* Grow the cache only if we need values beyond what we have. */
    idx_t old = (idx_t)cache.size();      /* first index not yet computed */
    if (n >= old) {
        cache.resize(n + 1);              /* make room for new entries */

        for (idx_t i = old; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 41 && i >= j; ++j)
                sum += cache[i - j];

            seq_t term = (seq_t)34 * sum;

            /* Add the b(n) contribution. */
            if (i == 0)          term += (seq_t)1;
            else if (i <= 41)    term += (seq_t)2;
            else if (i == 42)    term += (seq_t)1;

            /* Subtract the a(n-42) contribution if appropriate. */
            if (i >= 42)
                term -= (seq_t)595 * cache[i - 42];

            cache[i] = term;
        }
    }

    return cache[n];
#else
    /* C implementation – use a dynamically allocated array. */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;          /* highest index currently cached */

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);             /* OOM */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cached_max = 0;
    }

    if (n > cached_max) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);               /* OOM */
        cache = tmp;

        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 41 && i >= j; ++j)
                sum += cache[i - j];

            seq_t term = (seq_t)34 * sum;

            /* b(n) contribution. */
            if (i == 0)          term += (seq_t)1;
            else if (i <= 41)    term += (seq_t)2;
            else if (i == 42)    term += (seq_t)1;

            /* a(n-42) contribution. */
            if (i >= 42)
                term -= (seq_t)595 * cache[i - 42];

            cache[i] = term;
        }
        cached_max = n;
    }

    return cache[n];
#endif
}
