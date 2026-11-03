/*
 * A092627  Primes with exactly two nonprime digits.
 * https://oeis.org/A092627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092627: primes with exactly two nonprime digits, offset 1 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Count how many decimal digits of x are nonprime (0,1,4,6,8,9). */
static int count_nonprime_digits(uint64_t x)
{
    int cnt = 0;
    while (x > 0) {
        int d = x % 10;
        if (d == 0 || d == 1 || d == 4 || d == 6 || d == 8 || d == 9)
            ++cnt;
        x /= 10;
    }
    return cnt;
}

/* Cache of computed terms.  Results are stored in increasing order. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next integer to test for primality.  Starts at 2. */
static uint64_t next_candidate = 2;

/* Allocate or grow the cache array. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_cap < needed) {
        cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* Append a new term to the cache. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap)
        ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate terms until the cache contains at least n terms. */
static void generate_terms(idx_t n)
{
    while ((idx_t)cache_len < n) {
        if (is_prime(next_candidate)) {
            if (count_nonprime_digits(next_candidate) == 2)
                append_cache((seq_t)next_candidate);
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    generate_terms(n);
    return cache[n - 1];
}
