/*
 * A152505  1/10 of the number of permutations of 4 indistinguishable copies
 *          of 1..n with exactly 3 local maxima.
 * https://oeis.org/A152505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 211*a(n-1) - 13060*a(n-2) + 319850*a(n-3) * -
 *            3093125*a(n-4) + 12831875*a(n-5) - 19293750*a(n-6), * for n > 6.
 *            * Initial values: * a(1)=0, a(2)=3, a(3)=1008, a(4)=172573, *
 *            a(5)=24118698, a(6)=3148308323
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache_vec;
    static idx_t current = 0;

    /* ensure capacity */
    if (cache_vec.size() < n + 1)
        cache_vec.resize(n + 1);

    /* initialise first six terms once */
    if (current < 1 && n >= 1) { cache_vec[1] = (seq_t)0;    current = 1; }
    if (current < 2 && n >= 2) { cache_vec[2] = (seq_t)3;    current = 2; }
    if (current < 3 && n >= 3) { cache_vec[3] = (seq_t)1008;  current = 3; }
    if (current < 4 && n >= 4) { cache_vec[4] = (seq_t)172573; current = 4; }
    if (current < 5 && n >= 5) { cache_vec[5] = (seq_t)24118698; current = 5; }
    if (current < 6 && n >= 6) { cache_vec[6] = (seq_t)3148308323; current = 6; }

    /* compute missing values */
    for (idx_t i = current + 1; i <= n; ++i) {
        seq_t val = (seq_t)211 * cache_vec[i - 1];
        val += (seq_t)319850 * cache_vec[i - 3];
        val += (seq_t)12831875 * cache_vec[i - 5];
        val -= (seq_t)13060 * cache_vec[i - 2];
        val -= (seq_t)3093125 * cache_vec[i - 4];
        val -= (seq_t)19293750 * cache_vec[i - 6];
        cache_vec[i] = val;
    }
    current = n;
    return cache_vec[n];
#else
    /* C implementation using malloc / memcpy */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t current = 0;

    if (n < 1) return (seq_t)0;   /* below offset – not specified */

    /* enlarge cache if necessary */
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise first six terms once */
    if (current < 1 && n >= 1) { cache[1] = (seq_t)0;    current = 1; }
    if (current < 2 && n >= 2) { cache[2] = (seq_t)3;    current = 2; }
    if (current < 3 && n >= 3) { cache[3] = (seq_t)1008;  current = 3; }
    if (current < 4 && n >= 4) { cache[4] = (seq_t)172573; current = 4; }
    if (current < 5 && n >= 5) { cache[5] = (seq_t)24118698; current = 5; }
    if (current < 6 && n >= 6) { cache[6] = (seq_t)3148308323; current = 6; }

    /* compute missing values */
    for (idx_t i = current + 1; i <= n; ++i) {
        seq_t val = (seq_t)211 * cache[i - 1];
        val += (seq_t)319850 * cache[i - 3];
        val += (seq_t)12831875 * cache[i - 5];
        val -= (seq_t)13060 * cache[i - 2];
        val -= (seq_t)3093125 * cache[i - 4];
        val -= (seq_t)19293750 * cache[i - 6];
        cache[i] = val;
    }
    current = n;
    return cache[n];
#endif
}
