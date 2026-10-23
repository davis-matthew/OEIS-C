/*
 * A144096  A positive integer n is included if at least one of the exponents
 *          of the prime-factorization of n does not occur anywhere in n when
 *          the exponents and n are represented in base 2.
 * https://oeis.org/A144096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 953, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   exponent_missing, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth positive integer with the property that
 * at least one exponent in its prime‑factorisation does NOT occur as a
 * contiguous substring of the binary representation of the number.
 * The implementation checks integers one by one, factors them with
 * factor_u64, converts the number and each exponent to binary, and
 * tests for substring absence.  Found terms are cached so subsequent
 * calls (in any order) immediately return cached values. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(...) */
#include "../utils/exponent_missing.h"

/* Convert a 64‑bit integer to an array of bits (MSB first). */
static void to_bin(uint64_t x, uint8_t *buf, size_t *len)
{
    if (x == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    size_t pos = 0;
    while (x) {
        buf[pos++] = (uint8_t)(x & 1);
        x >>= 1;
    }
    /* reverse to MSB first */
    for (size_t i = 0; i + i < pos; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    *len = pos;
}

/* Test whether integer n satisfies the inclusion condition. */
static bool included(uint64_t n)
{
    if (n <= 1) return false;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    uint8_t bin_n[64];
    size_t len_n;
    to_bin(n, bin_n, &len_n);

    for (size_t i = 0; i < cnt; ++i) {
        uint8_t bin_exp[64];
        size_t len_exp;
        to_bin(exps[i], bin_exp, &len_exp);
        if (exponent_missing(bin_n, len_n, bin_exp, len_exp))
            return true;   /* at least one exponent missing */
    }
    return false;          /* all exponents appear */
}

/* Cached sequence terms. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure cache has at least 'need' capacity. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public interface: a(n) returns the nth term of A144096. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset, harmless */

    /* Grow cache up to n terms. */
    if (cache_len < (size_t)n) {
        while (cache_len < (size_t)n) {
            while (!included(next_candidate)) ++next_candidate;
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
