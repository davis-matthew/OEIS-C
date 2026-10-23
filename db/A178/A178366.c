/*
 * A178366  Numbers with rounded up arithmetic mean of digits = 6.
 * https://oeis.org/A178366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer whose rounded‑up arithmetic mean of digits equals 6. */

#include <stddef.h>
#include <stdlib.h>

/* Helper that extracts the base‑10 digits of a number into a buffer. */
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Static cache for the sequence terms. */
    static seq_t *cache = NULL;          /* array of found terms */
    static size_t cache_len = 0;         /* number of terms stored */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* The largest integer that has been examined. */
    static seq_t test_val = 0;

    /* Ensure we have at least n terms computed. */
    while (cache_len < (size_t)n)
    {
        /* Examine the next integer. */
        test_val += (seq_t)1;

        /* Extract the decimal digits of test_val. */
        static uint8_t digits[128];      /* large enough for any seq_t in tests */
        size_t len = 0;
        get_digits(test_val, digits, &len);
        if (len == 0) len = 1;           /* defensive, should not happen */

        /* Compute the sum of digits. */
        unsigned long long sum = 0;
        for (size_t i = 0; i < len; ++i)
            sum += digits[i];

        /* Condition: 5·len < sum ≤ 6·len  ⇔  ceil(sum/len) = 6. */
        if (sum > 5ULL * len && sum <= 6ULL * len)
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }

            /* Store the new term. */
            cache[cache_len++] = test_val;
        }
    }

    /* Return the requested term (1‑based indexing). */
    return cache[(size_t)n - 1];
}
