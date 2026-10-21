/*
 * A061911  Square root of the sum of the digits of k^2 when this sum is a
 *          square.
 * https://oeis.org/A061911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A061911:  a(n) is the nth square root of the digit sum of k²
   when that digit sum is a perfect square, enumerated for increasing k. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned long long value.  */
static inline unsigned long long isqrt_uint64(unsigned long long x)
{
    unsigned long long low = 0, high = (x < 0xFFFFFFFFULL) ?
        (unsigned long long)(1ULL << ((sizeof(unsigned long long) * 8 + 1) / 2)) :
        0xFFFFFFFFULL;
    while (low < high) {
        unsigned long long mid = low + (high - low + 1) / 2;
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Compute sum of decimal digits of an unsigned __int128 number.  */
static inline unsigned long long digit_sum_uint128(unsigned __int128 x)
{
    unsigned long long sum = 0;
    while (x) {
        sum += (unsigned long long)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, safety only */

    /* Static storage for computed terms and state. */
    static seq_t *cache = NULL;     /* array of found roots */
    static idx_t cache_len = 0;     /* allocated array length (in terms) */
    static idx_t cache_count = 0;   /* number of terms actually stored */
    static idx_t next_k = 1;        /* next integer k to test */

    /* Grow the cache array if needed.  */
    while (cache_count < n) {
        /* Test the next k. */
        unsigned __int128 sq  = (unsigned __int128)next_k * next_k;
        unsigned long long sum = digit_sum_uint128(sq);
        unsigned long long r = isqrt_uint64(sum);
        if (r * r == sum) {          /* perfect square digit sum */
            /* Ensure enough space. */
            if (cache_count == cache_len) {
                idx_t new_len = cache_len ? cache_len * 2 : 8;
                while (new_len <= cache_count) new_len <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
                if (!tmp) return (seq_t)0;    /* allocation failure */
                cache = tmp;
                cache_len = new_len;
            }
            cache[cache_count] = (seq_t)r;
            cache_count++;
        }
        next_k++;   /* advance to next k regardless of whether it matched */
    }

    return cache[n - 1];
}
