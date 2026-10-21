/*
 * A236391  Positive numbers n such that 100*n^2/(100+n^2) are integers.
 * https://oeis.org/A236391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Enumerate divisors d of 10000.  For each d>100,
   if d‑100 is a perfect square n², include n.  The resulting
   list is finite (5 terms).  For n beyond the list, return 0. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: integer square root for uint64_t.  Works for all
   values up to 2⁶⁴-1, but here we only need it for values
   up to 10 000. */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* One‑time static workspace */
    static bool initialized = false;
    static seq_t cache[64];
    static size_t count = 0;

    if (!initialized) {
        const idx_t LIMIT = 10000;          /* 100² */
        size_t pos = 0;

        for (idx_t i = 1; (i * i) <= LIMIT; ++i) {
            if (LIMIT % i == 0) {
                idx_t d1 = i;
                idx_t d2 = LIMIT / i;

                if (d1 > 100) {
                    uint64_t diff = (uint64_t)d1 - 100ULL;
                    uint64_t s = isqrt_uint64(diff);
                    if (s * s == diff)
                        cache[pos++] = (seq_t)s;
                }
                if (d2 != d1 && d2 > 100) {
                    uint64_t diff = (uint64_t)d2 - 100ULL;
                    uint64_t s = isqrt_uint64(diff);
                    if (s * s == diff)
                        cache[pos++] = (seq_t)s;
                }
            }
        }

        /* Sort the array of terms (it has at most a handful of elements). */
        if (pos > 1) {
            for (size_t i = 1; i < pos; ++i) {
                seq_t key = cache[i];
                size_t j = i;
                while (j > 0 && cache[j - 1] > key) {
                    cache[j] = cache[j - 1];
                    --j;
                }
                cache[j] = key;
            }
        }
        count = pos;
        initialized = true;
    }

    if (n == 0 || (idx_t)n > count)
        return (seq_t)0;
    return cache[(size_t)n - 1];
}
