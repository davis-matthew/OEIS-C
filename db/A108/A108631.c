/*
 * A108631  Semiprimes with nonprime digits (no digits 2,3,5,7 in semiprimes).
 * https://oeis.org/A108631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth semiprime whose decimal digits are not 2,3,5,7
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* true if x contains none of the forbidden digits 2,3,5,7 */
static bool allowed_digits(uint64_t x)
{
    while (x > 0) {
        uint8_t d = x % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7)
            return false;
        x /= 10;
    }
    return true;
}

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* sequence a(n) with offset 1 */
seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static uint64_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static uint64_t last_checked = 4;     /* next integer to test */

    if (n == 0)      /* safety for 0‑based calls, though offset is 1 */
        return 0;

    while (cache_size < n) {
        if (allowed_digits(last_checked) && is_semiprime_uint64(last_checked)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? (cache_cap << 1) : 16;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;
    }

    return (seq_t)cache[n - 1];
}
