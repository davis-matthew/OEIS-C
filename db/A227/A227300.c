/*
 * A227300  Rising diagonal sums of triangle of Fibonacci polynomials (rows
 *          displayed as centered text).
 * https://oeis.org/A227300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - a(n-6) (n ≥ 7) a(1) = 1, a(2) = 2,
 *            a(3) = 2, a(4) = 3, a(5) = 7, a(6) = 11
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// A227300: Rising diagonal sums of triangle of Fibonacci polynomials

#include <stdlib.h>
#include <stddef.h>

/* Pre‑computed initial terms for indices 1..6 */
static const seq_t base_vals[6] = {1, 2, 2, 3, 7, 11};

/* Cached sequence values: cache[n-1] holds a(n) for n ≥ 1 */
static seq_t *cache_ptr = NULL;
static size_t cache_len = 0;

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0 */

    /* Convert idx_t to size_t for array indices (n fits in a 64‑bit unsigned) */
    size_t target = (size_t)n;     /* maximum index we need to compute */

    /* Allocate or expand the cache if necessary */
    if (target > cache_len) {
        size_t new_len = target;
        /* realloc returns void* in C and void* in C++ ; explicit cast required */
        seq_t *new_cache = (seq_t *)realloc(cache_ptr, new_len * sizeof(seq_t));
        if (!new_cache)           /* allocation failed – return zero */
            return 0;
        /* Compute newly required entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t val;
            if (i < 6) {                 /* initial terms */
                val = base_vals[i];
            } else {                     /* recurrence */
                seq_t t1 = new_cache[i - 1]; /* a(i) = a(i-1) ... */
                seq_t t3 = new_cache[i - 3]; /* a(i-3)   */
                seq_t t6 = new_cache[i - 6]; /* a(i-6)   */
                val = t1 + (t3 + t3) - t6;   /* a(n-1)+2*a(n-3)-a(n-6) */
            }
            new_cache[i] = val;
        }
        cache_ptr = new_cache;
        cache_len = new_len;
    }

    return cache_ptr[target - 1];   /* a(n) stored at index n-1 */
}
