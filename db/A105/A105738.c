/*
 * A105738  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=5.
 * https://oeis.org/A105738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=1, a(2)=5.
 * For n>2, a(n) is the minimal positive integer such that
 *      a(n-1)^2 + 4 * a(n-2) * a(n)   is a perfect square.
 *
 * The function caches computed values for efficient random access.
 */
#include <stdlib.h>

/* Integer square root for arbitrary-sized seq_t (guaranteed to fit in seq_t). */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0, high = x, mid, sq;
    while (low <= high) {
        mid = (low + high) >> 1;
        sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;               /* floor(sqrt(x)) */
}

/* Cached sequence values. */
static seq_t *cache       = NULL;
static idx_t  cache_size  = 0;   /* number of elements stored (>= current max n) */
static idx_t  cache_cap   = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        exit(EXIT_FAILURE);  /* allocation failure */
    cache = newp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)    /* out of range, undefined by OEIS (offset 1) */
        return 0;

    /* Base terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* Extend the cache up to n. */
    if (n > cache_size) {
        ensure_cache_cap(n);
        /* Initialize first two elements if cache is empty. */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;           /* a(1) */
            cache[1] = (seq_t)5;           /* a(2) */
            cache_size = 2;
        }
        /* Now compute terms from cache_size+1 up to n. */
        seq_t a_nm2 = cache[cache_size - 2];   /* a(current-2) */
        seq_t a_nm1 = cache[cache_size - 1];   /* a(current-1) */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t a_candidate = 1;
            while (1) {
                seq_t val = a_nm1 * a_nm1 + (seq_t)4 * a_nm2 * a_candidate;
                seq_t s = isqrt(val);
                if (s * s == val)
                    break;
                ++a_candidate;
            }
            cache[cache_size] = a_candidate;
            ++cache_size;
            a_nm2 = a_nm1;
            a_nm1 = a_candidate;
        }
    }

    return cache[n - 1];
}
