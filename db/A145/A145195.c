/*
 * A145195  Odd composite numbers n with property that at least one prime
 *          divisor p of n is a substring of the binary representation of n.
 * https://oeis.org/A145195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 71 terms through n = 4997 (2026-11-03)
 * Helpers:   is_prime, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145195: odd composite numbers n such that some prime divisor p of n
   appears as a binary substring of n.  The function returns the nth such
   term by incremental search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/* Convert an unsigned 64‑bit integer to a binary string.
   buf must be large enough (at least 65 bytes).  The string is
   null‑terminated and most‑significant bit first. */
static size_t to_bin_str(uint64_t x, char *buf)
{
    size_t pos = 0;
    do {
        buf[pos++] = (x & 1ULL) ? '1' : '0';
        x >>= 1;
    } while (x);
    /* reverse to correct order */
    for (size_t i = 0, j = pos - 1; i < j; ++i, --j) {
        char t = buf[i];
        buf[i] = buf[j];
        buf[j] = t;
    }
    buf[pos] = '\0';
    return pos;
}

/* Return true if binary representation of p occurs as a contiguous
   substring of the binary representation of n. */
static bool contains_prime_bits(uint64_t n, uint64_t p)
{
    char bin_n[65], bin_p[65];
    to_bin_str(n, bin_n);
    to_bin_str(p, bin_p);
    return strstr(bin_n, bin_p) != NULL;
}

/* Test whether an odd composite number n satisfies the
   property.  n is assumed odd and > 2. */
static bool satisfies(uint64_t n)
{
    uint64_t temp = n;
    /* factor n into distinct prime factors */
    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        if (contains_prime_bits(n, p))
            return true;
        while (temp % p == 0)
            temp /= p;
    }
    return false;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t next_candidate = 3;  /* first odd number to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_cnt < (size_t)n) {
        /* Skip primes (and 1). */
        if (!is_prime((idx_t)next_candidate)) {
            if (satisfies(next_candidate)) {
                if (cache_cnt >= cache_cap) {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_cnt++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;  /* only odd numbers */
    }

    return cache[(size_t)n - 1];
}
