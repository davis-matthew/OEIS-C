/*
 * A218227  Hilltop maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any
 *          horizontal, vertical or antidiagonal neighbor in a random 0..2 n X
 *          2 array.
 * https://oeis.org/A218227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+3*a(n-2)+3*a(n-3)+3*a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)
 *            Initial values: a(1)=3, a(2)=15, a(3)=61, a(4)=241, a(5)=961,
 *            a(6)=3839, a(7)=15327, a(8)=61185, a(9)=244257 The function
 *            memoises computed terms in a static array
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000

/* Known first nine terms as 64‑bit constants (wide enough for all seq_t). */
static const unsigned long long base_vals[9] = {
    3ULL, 15ULL, 61ULL, 241ULL, 961ULL,
    3839ULL, 15327ULL, 61185ULL, 244257ULL
};

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;    /* number of terms cached (indices 0 .. cache_len-1) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n == 0) return 0;   /* defined only as a safety measure */

    /* Initialise the cache with the first nine terms on the first call. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 9; ++i) {
            cache[i] = (seq_t)base_vals[i];
        }
        cache_len = 9;  /* we have terms a(1) .. a(9) stored */
    }

    /* If requested term is already cached, return it directly. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Compute terms up to n and cache them. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to term a(i+1).  We have i >= 9 here. */
        seq_t sum4 = cache[i - 1] + cache[i - 2] + cache[i - 3] + cache[i - 4];
        sum4 = (seq_t)3 * sum4;
        seq_t next = sum4 + cache[i - 5] + cache[i - 6] +
                     cache[i - 7] + cache[i - 8] + cache[i - 9];
        cache[i] = next;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
