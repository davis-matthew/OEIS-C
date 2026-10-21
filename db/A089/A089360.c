/*
 * A089360  Prime worms.
 * https://oeis.org/A089360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal digits satisfy the
 *            prime‑worm conditions: at least 3 digits, identical first and
 *            last digits, no two adjacent digits equal, and all adjacent
 *            absolute differences are the same. This implementation searches
 *            primes to produce the term
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_digits, is_prime_big, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/get_digits.h"

/* Helper that tests a prime for the prime‑worm conditions. */
static bool is_worm_prime(seq_t p)
{
    /* Must be at least 3 decimal digits. */
    if (p < (seq_t)101)
        return false;

    /* Quick reject if adjacent digits are equal. */
    if (!no_consecutive_equal_digits(p))
        return false;

    /* Gather decimal digits in most‑significant order. */
    uint8_t buf[128];
    size_t len = 0;
    get_digits(p, buf, &len);
    if (len < 3)
        return false;

    /* First and last digits must match. */
    if (buf[0] != buf[len - 1])
        return false;

    /* Constant absolute difference between successive digits. */
    int diff = (buf[0] > buf[1]) ? buf[0] - buf[1] : buf[1] - buf[0];
    if (diff == 0)           /* adjacent digits already checked, but keep safety. */
        return false;
    for (size_t i = 1; i + 1 < len; ++i) {
        int cur = (buf[i] > buf[i + 1]) ? buf[i] - buf[i + 1] : buf[i + 1] - buf[i];
        if (cur != diff)
            return false;
    }

    /* Finally verify primality. */
    return is_prime_big(p);
}

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current candidate to test; start at the smallest 3‑digit prime. */
    static seq_t next_cand = (seq_t)101;

    while (cache_len < n) {
        seq_t cand = next_cand;
        /* Advance to the next odd number. All primes >2 are odd. */
        next_cand += (seq_t)2;

        if (is_worm_prime(cand)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[(size_t)(n - 1)];
}
