/*
 * A171584  Squares such that swapping some pair of digits gives a prime.
 * https://oeis.org/A171584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n‑th perfect square that can become a prime by swapping *
 *            any pair of its decimal digits
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_DIGITS 32

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Check whether square `v` has a digit swap that yields a prime. */
static bool square_has_prime_swap(seq_t v)
{
    static uint8_t digits[MAX_DIGITS];
    static seq_t  pow10[MAX_DIGITS];
    static bool pow_init = false;

    if (!pow_init) {                 /* initialise powers of ten once */
        pow10[0] = (seq_t)1;
        for (int i = 1; i < MAX_DIGITS; ++i) {
            pow10[i] = pow10[i - 1] * (seq_t)10;
        }
        pow_init = true;
    }

    /* Extract digits of v in little‑endian order. */
    seq_t tmp = v;
    int len = 0;
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }
    if (len == 0) digits[len++] = 0;            /* handle v == 0 */

    if (len > MAX_DIGITS) len = MAX_DIGITS;    /* safety, though not needed */

    /* Try every unordered pair of digit positions. */
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            int diff = (int)digits[i] - (int)digits[j];
            seq_t pdiff = pow10[j] - pow10[i];
            seq_t new_val = v;
            if (diff > 0) {
                new_val += (seq_t)diff * pdiff;
            } else if (diff < 0) {
                new_val -= (seq_t)(-diff) * pdiff;
            }
            if (is_prime_big(new_val)) {
                return true;
            }
        }
    }
    return false;
}

/* Retrieve the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* irrelevant for OEIS offset 1 */
    }

    /* Memoised cache of qualifying squares. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  m_max_used = 0;       /* largest m checked so far */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++m_max_used;
        seq_t square = (seq_t)m_max_used * (seq_t)m_max_used;
        if (square_has_prime_swap(square)) {
            if (cache_len == cache_cap) {
                size_t old_cap = cache_cap;
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                cache = seq_realloc(cache, old_cap, cache_cap);
            }
            cache[cache_len++] = square;
        }
    }

    return cache[n - 1];
}
