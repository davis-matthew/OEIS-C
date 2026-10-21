/*
 * A083971  Reverse of k concatenated with k, divided by k, where k =
 *          A083970(n).
 * https://oeis.org/A083971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ( reverse(k) * 10^d + k ) / k, where k = A083970(n)
 * Range:     u8 n <= 12, u16 n <= 168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, reverse_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/reverse_uint64.h"
#include "../utils/digit_count.h"

/* Test whether k satisfies k | reverse(k) * 10^d */
static bool k_is_good(uint64_t k)
{
    int d = digit_count(k);
    uint64_t revk = reverse_uint64(k);

    /* compute 10^d mod k */
    uint64_t powmod = 1 % k;
    for (int i = 0; i < d; ++i) {
        powmod = (unsigned __int128)powmod * 10ULL % k;
    }

    uint64_t revmod = revk % k;
    uint64_t prod = (unsigned __int128)revmod * powmod % k;
    return prod == 0;
}

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t last_k = 0;          /* last integer checked */

    if (n == 0) {
        return 0;                       /* offset is 1; n==0 is unused */
    }

    /* Ensure we have at least n entries */
    while (cache_size < (size_t)n) {
        ++last_k;
        if (!k_is_good(last_k)) {
            continue;                   /* skip this k */
        }

        /* Compute a value for this k */
        uint64_t revk = reverse_uint64(last_k);
        int d = digit_count(last_k);

        /* compute 10^d as unsigned long long (fits for d <= 19, which is enough for n <= 10000) */
        unsigned long long pow10d = 1ULL;
        for (int i = 0; i < d; ++i) {
            pow10d *= 10ULL;
        }

        /* Use 128-bit for intermediate to avoid overflow */
        unsigned __int128 num = (unsigned __int128)revk * pow10d + (unsigned __int128)last_k;
        unsigned __int128 a128 = num / (unsigned __int128)last_k;
        seq_t a_val = (seq_t)a128;

        /* Expand cache if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed: fall back to zero */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = a_val;
    }

    return cache[n - 1];
}
