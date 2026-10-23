/*
 * A184546  Number of (n+2) X 9 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) (for n > 10) with initial values a(1) … a(10) given
 *            below
 * Range:     u8 none, u16 n <= 7, u32 n <= 41, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[10] = {
    539ULL, 1513ULL, 3461ULL, 7307ULL, 14767ULL,
    28844ULL, 54543ULL, 99872ULL, 177207ULL, 305112ULL
};

static const int coeff[10] = {
    10, -45, 120, -210, 252,
    -210, 120, -45, 10, -1
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* sequence is defined for n ≥ 1 */

    /* Lazy static cache of computed values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* If no cache yet build it with at least the first 10 values */
    if (cache_len == 0) {
        cache_cap = n > 10 ? (size_t)n : 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_len = 10;
    }

    /* Grow cache capacity if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap = new_cap ? new_cap * 2 : 10;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute further terms up to n */
    while (cache_len < (size_t)n) {
        size_t cur = cache_len + 1;          /* 1‑based index of the
                                                term to compute */
        seq_t val = 0;
        for (int k = 1; k <= 10; ++k) {
            seq_t term = cache[cur - k - 1];
            int c = coeff[k - 1];
            if (c >= 0)
                val += term * (seq_t)c;
            else
                val -= term * (seq_t)(-c);
        }
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n - 1];
}
