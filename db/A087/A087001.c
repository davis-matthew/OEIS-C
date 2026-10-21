/*
 * A087001  Left half of periodic part of decimal expansion of 1/p for those
 *          primes having a periodic part of even length.
 * https://oeis.org/A087001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(A086999(n)/10^A087000(n))
 * where A086999(n) is the decimal number formed by the periodic part
 * of 1/p (for the n‑th prime p with even period length), and
 * A087000(n) is the half length of that period.
 * The implementation computes a(n) by generating the decimal expansion
 * of 1/p, determining its period, and extracting the left half of the
 * period (ignoring any leading zeros, but keeping the number of
 * trailing zeros necessary to make the division exact).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_size = 0;    /* number of terms stored */

    /* Cached information for prime search */
    static idx_t next_prime = 2;     /* next candidate prime to test */

    /* Ensure cache can hold at least n terms */
    while (cache_size < n)
    {
        /* Search for next prime */
        idx_t p = next_prime;
        while (!is_prime(p))
            ++p;
        next_prime = p + 1;

        /* Skip primes 2 and 5 which have terminating decimals */
        if (p == 2 || p == 5)
            continue;

        /* Generate decimal digits of 1/p until the period repeats */
        idx_t r = 1 % p; /* initial remainder */
        size_t digits_len = 0;
        size_t digits_cap = 64;
        uint8_t *digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) return 0; /* memory error, fallback to 0 */

        while (true)
        {
            unsigned __int128 rem = (unsigned __int128)r * 10;
            uint8_t d = (uint8_t)(rem / p);
            r = (idx_t)(rem % p);
            if (digits_len == digits_cap)
            {
                digits_cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
                if (!tmp)
                {
                    free(digits);
                    return 0; /* memory error, fallback to 0 */
                }
                digits = tmp;
            }
            digits[digits_len++] = d;
            if (r == 1)
                break;
        }

        /* Period length */
        size_t L = digits_len;
        if (L == 0)
        {
            free(digits);
            continue; /* shouldn't happen */
        }

        /* Skip odd period lengths */
        if (L & 1)
        {
            free(digits);
            continue;
        }
        size_t half = L >> 1;

        /* Count leading zero digits of the period */
        size_t Lz = 0;
        while (Lz < L && digits[Lz] == 0)
            ++Lz;

        /* Length of the prefix that represents the left half of the period
           after removing trailing zeros (i.e., removing all trailing digits
           that would be part of the right half). */
        size_t prefix_len = half + Lz;

        /* Build the answer as an integer from the prefix digits */
        seq_t ans = 0;
        for (size_t i = 0; i < prefix_len; ++i)
            ans = ans * 10 + digits[i];

        free(digits);

        /* Store the result in the cache, extending it if necessary */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = ans;
    }

    return cache[n - 1];
}
