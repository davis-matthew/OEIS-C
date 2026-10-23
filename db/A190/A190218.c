/*
 * A190218  Numbers all of whose divisors are numbers whose decimal digits are
 *          in strictly increasing order.
 * https://oeis.org/A190218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 143, u32 n <= 163, u64 n <= 163,
 *            u128 n <= 163, big any n
 * Helpers:   inc_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190218: numbers all of whose divisors have strictly
 * increasing decimal digits.  Computed by exhaustive search. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/inc_digits_uint64.h"

/* Test whether every divisor of n has strictly increasing digits. */
static bool good_candidate(uint64_t n)
{
    uint64_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;

    for (uint64_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = n / d;
            if (!inc_digits_uint64(d1))
                return false;
            if (d1 != d2 && !inc_digits_uint64(d2))
                return false;
        }
    }
    return true;
}

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Return the nth term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t cache_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Generate all numbers with strictly increasing decimal digits
         * (There are 2^9‑1 = 511 such numbers using digits 1…9). */
        const int max_mask = 1 << 9;   /* digits 1..9 */
        uint64_t *tmp = (uint64_t *)malloc((size_t)max_mask * sizeof(uint64_t));
        size_t cnt = 0;

        for (int mask = 1; mask < max_mask; ++mask) {
            uint64_t num = 0;
            for (int bit = 0; bit < 9; ++bit) {
                if (mask & (1 << bit))
                    num = num * 10 + (uint64_t)(bit + 1);  /* digits 1..9 */
            }
            if (good_candidate(num))
                tmp[cnt++] = num;
        }

        /* Sort the qualifying numbers to obtain the OEIS order. */
        qsort(tmp, cnt, sizeof(uint64_t), cmp_uint64);

        cache = tmp;
        cache_count = cnt;
        initialized = true;
    }

    if (n == 0 || (size_t)n > cache_count)
        return (seq_t)0;   /* out of range: no term defined */

    return (seq_t)cache[(size_t)n - 1];
}
