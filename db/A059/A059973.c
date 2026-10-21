/*
 * A059973  Expansion of x*(1 + x - 2*x^2) / ( 1 - 4*x^2 - x^4).
 * https://oeis.org/A059973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 63, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-2) + a(n-4)   with   a(0)=0, a(1)=a(2)=1, a(3)=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for arbitrary‑precision types */
    static std::vector<seq_t> cache_vec;
    /* initialise base cases once */
    if (cache_vec.size() < 4) {
        cache_vec.resize(4);
        cache_vec[0] = 0;
        cache_vec[1] = 1;
        cache_vec[2] = 1;
        cache_vec[3] = 2;
    }
    /* extend cache if necessary */
    if ((idx_t)cache_vec.size() <= n) {
        size_t old_size = cache_vec.size();
        cache_vec.resize((size_t)n + 1);
        for (size_t i = old_size; i <= (size_t)n; ++i) {
            seq_t v;
            if (i == 0) {
                v = 0;
            } else if (i == 1 || i == 2) {
                v = 1;
            } else if (i == 3) {
                v = 2;
            } else {
                v = 4 * cache_vec[i - 2] + cache_vec[i - 4];
            }
            cache_vec[i] = v;
        }
    }
    return cache_vec[(size_t)n];
#else
    /* C implementation using a malloc/realloc cache */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* number of valid entries in cache   */
    static idx_t cap = 0;           /* current capacity of the allocation */

    if (n >= (idx_t)size) {         /* need new entries */
        idx_t new_size = n + 1;
        if (cap < new_size) {       /* enlarge allocation */
            idx_t new_cap = new_size > 2 * cap ? new_size : 2 * cap;
            seq_t *tmp = realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) return 0;     /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        /* initialise new entries */
        for (idx_t i = size; i < new_size; ++i) {
            seq_t v;
            if (i == 0) {
                v = 0;
            } else if (i == 1 || i == 2) {
                v = 1;
            } else if (i == 3) {
                v = 2;
            } else {
                v = 4 * cache[i - 2] + cache[i - 4];
            }
            cache[i] = v;
        }
        size = new_size;
    }
    return cache[n];
#endif
}
