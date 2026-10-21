/*
 * A207809  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207809 – Number of 4×n 0/1 arrays avoiding the forbidden patterns
 *
 *  Recurrence (for n ≥ 7):
 *      a(n) = 2*a(n-1) + 13*a(n-2) + 4*a(n-3) – 12*a(n-4) + a(n-5) + a(n-6)
 *
 *  Initial values (n = 1 … 6):
 *      a(1) = 10,  a(2) = 100,  a(3) = 370,  a(4) = 1970,
 *      a(5) = 9040, a(6) = 43990
 *
 *  The function memoises computed terms so it works efficiently for any
 *  order of calls with n ≥ 1.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Undefined for n = 0 */
    if (n == 0)
        return 0;

    /* Static cache for already computed terms */
    static seq_t *cache = NULL;   /* pointer to array of seq_t   */
    static idx_t cap   = 0;       /* allocated capacity (elements) */
    static idx_t sz    = 0;       /* number of terms actually stored */

    /* Ensure capacity: need at least n elements, but also at least 6
       to hold the initial values after the first call. */
    if (cache == NULL || cap < ((n > 6) ? n : 6)) {
        idx_t new_cap = (n > 6) ? n : 6;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed */
            return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first six values once */
    if (sz < 6) {
        static const seq_t init[6] = {10, 100, 370, 1970, 9040, 43990};
        for (idx_t i = sz; i < 6; ++i)
            cache[i] = init[i];
        sz = 6;
    }

    /* Return directly if the requested value is already cached */
    if (n <= sz)
        return cache[n - 1];

    /* Compute remaining terms using the recurrence */
    for (idx_t i = sz; i < n; ++i) {
        seq_t v = 2 * cache[i - 1] + 13 * cache[i - 2] + 4 * cache[i - 3];
        v = v + cache[i - 5] + cache[i - 6];
        v = v - (12 * cache[i - 4]);   /* safe subtraction, result is positive */
        cache[i] = v;
    }

    sz = n;
    return cache[n - 1];
}
