/*
 * A070328  Numbers in which inserting, prefixing or suffixing a 1 anywhere
 *          yields only composite numbers.
 * https://oeis.org/A070328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070328: Numbers in which inserting, prefixing or suffixing a 1 anywhere
   yields only composite numbers.  The function returns the n-th term, with a(1)=2. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Store computed terms for repeated queries */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

/* The next integer to test when extending the sequence */
static uint64_t next_candidate = 2;

/* Simple deterministic trial‑division primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Return true if all numbers obtained by inserting a single decimal '1'
   into num are composite (i.e., none is prime). */
static bool is_good_candidate(uint64_t num)
{
    char s[32];
    int len = snprintf(s, sizeof s, "%llu", (unsigned long long)num);
    if (len < 0) return false;          /* should never happen */

    char t[34];
    for (int pos = 0; pos <= len; ++pos) {
        /* build new string: prefix part, '1', suffix part */
        memcpy(t, s, pos);
        t[pos] = '1';
        memcpy(t + pos + 1, s + pos, len - pos);
        t[len + 1] = '\0';

        unsigned long long inserted = strtoull(t, NULL, 10);
        if (is_prime_uint64(inserted))
            return false;
    }
    return true;
}

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; real sequence offset is 1 */

    if (n <= cache_size) return cache[n - 1];

    ensure_capacity((size_t)n);

    while (cache_size < n) {
        if (is_good_candidate(next_candidate)) {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
