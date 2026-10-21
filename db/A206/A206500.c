/*
 * A206500  Number of nX3 0..7 arrays with no element equal to another within
 *          a city block distance of two, and new values 0..7 introduced in
 *          row major order.
 * https://oeis.org/A206500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 55*a(n-1) + 370*a(n-2) - 8556*a(n-3) + 15608*a(n-4) +
 *            70028*a(n-5) - 175808*a(n-6) + 98304*a(n-7) for n>9 Initial
 *            values: a(1)=1, a(2)=4, a(3)=65, a(4)=3130, a(5)=178998,
 *            a(6)=10502458, a(7)=618242826, a(8)=36410671102,
 *            a(9)=2144479610070
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] is unused, indices start at 1 */
static size_t cache_cap = 0;  /* allocated capacity in terms of indices+1 */
static size_t cache_len = 0;  /* next free index (i.e. cache_len-1 is last used) */

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* idx = n (idx_t is unsigned)  */
    size_t needed = idx + 1;         /* we need indices up to idx inclusive */

    /* Ensure allocated capacity is enough */
    if (needed > cache_cap) {
        size_t new_cap = needed * 2; /* grow by factor 2 for future calls */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First call: initialise the first nine terms */
    if (cache_len == 0) {
        /* make sure capacity is at least 10 */
        if (cache_cap < 10) {
            size_t new_cap = 10;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)4;
        cache[3]  = (seq_t)65;
        cache[4]  = (seq_t)3130;
        cache[5]  = (seq_t)178998;
        cache[6]  = (seq_t)10502458;
        cache[7]  = (seq_t)618242826;
        cache[8]  = (seq_t)36410671102;
        cache[9]  = (seq_t)2144479610070;
        cache_len = 10; /* next free index after 9 */
    }

    /* If the requested term is already computed, just return it */
    if (idx + 1 <= cache_len) {
        return cache[idx];
    }

    /* Compute missing terms up to idx using the recurrence */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val = (seq_t)55   * cache[i-1]    +
                    (seq_t)370  * cache[i-2]    -
                    (seq_t)8556 * cache[i-3]    +
                    (seq_t)15608* cache[i-4]    +
                    (seq_t)70028* cache[i-5]    -
                    (seq_t)175808*cache[i-6]    +
                    (seq_t)98304*cache[i-7];
        cache[i] = val;
    }

    cache_len = idx + 1;   /* update next free index */
    return cache[idx];
}
