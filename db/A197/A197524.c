/*
 * A197524  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,4,1,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 19*a(n-3) + 10*a(n-4) + 21*a(n-5) +
 *            31*a(n-6) - 99*a(n-7) - 9*a(n-8) + 16*a(n-9); initial values:
 *            a(1)=1, a(2)=3, a(3)=32, a(4)=69, a(5)=308, a(6)=1444,
 *            a(7)=5048, a(8)=20265, a(9)=83277
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined for n<1, but return 0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* how many terms are computed */
    static size_t cache_cap = 0;   /* current capacity of cache array */

    /* Base values for n = 1..9 */
    const seq_t base_vals[9] = {1, 3, 32, 69, 308, 1444, 5048, 20265, 83277};

    /* Initialise cache with base values on first call */
    if (cache_len == 0) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base_vals[i];
        cache_len = 9;
    }

    /* Return value if it is already known */
    if (n <= cache_len)
        return cache[n - 1];

    /* Resize cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Recurrence coefficients */
    static const int coeff[9] = {2, 2, 19, 10, 21, 31, -99, -9, 16};

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Cache indices are 0‑based: a(k) is cache[k-1] */
        idx_t base_idx = i - 2;          /* index of a(i-1) in cache */
        seq_t pos = 0, neg = 0;          /* positive and negative sums */

        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[base_idx - k];
            if (coeff[k] >= 0)
                pos += term * (seq_t)coeff[k];
            else
                neg += term * (seq_t)(-coeff[k]);
        }
        cache[cache_len] = pos - neg;   /* final value for a(i) */
        ++cache_len;
    }

    return cache[n - 1];
}
