/*
 * A164983  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28·a(n-1)+…+28·a(n-7) – 406·a(n-8) + c_n where c_0 = c_8
 *            = 1, c_1 = … = c_7 = 2, c_n = 0 for n ≥ 9. (Recurrence derived
 *            from the rational generating function of A164983.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for memoised values.  The array grows automatically as needed. */
static seq_t *cache   = NULL;   /* computed terms          */
static size_t  cache_cap = 0;   /* allocated capacity      */
static idx_t   cache_size = 0;  /* number of terms stored */

/* Ensure the cache can hold indices up to n (inclusive).  Handles both
   C and C++ allocations for arbitrary seq_t types. */
static void
ensure_capacity(idx_t n)
{
    if (cache && cache_cap >= (size_t)(n + 1))
        return;   /* already large enough */

    /* Double capacity until it exceeds n+1 */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;   /* multiply by 2 */

    seq_t *new_ptr;
#ifdef __cplusplus
    new_ptr = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_ptr[i] = cache[i];
        delete[] cache;
    }
#else
    new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!new_ptr)
        abort(); /* allocation failed */

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function – memoised, works for any seq_t type. */
seq_t
a(idx_t n)
{
    /* Initialise cache if needed. */
    if (!cache)
        ensure_capacity(n);

    /* Compute terms up to n if not already present. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Determine the constant term c_i from the numerator of the G.f. */
        seq_t cn;
        if (i == 0)
            cn = (seq_t)1;
        else if (i <= 7)
            cn = (seq_t)2;
        else if (i == 8)
            cn = (seq_t)1;
        else
            cn = (seq_t)0;

        seq_t sum = cn;                    /* start with c_i */

        /* Add 28·a(i-k) for k = 1..7, if i ≥ k. */
        for (idx_t k = 1; k <= 7; ++k) {
            if (i >= k)
                sum += (seq_t)28 * cache[i - k];
        }

        /* Subtract 406·a(i-8) for i ≥ 8. */
        if (i >= 8)
            sum -= (seq_t)406 * cache[i - 8];

        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
