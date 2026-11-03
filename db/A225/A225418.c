/*
 * A225418  Composite numbers which contain their sum of the distinct prime
 *          factors (sopf function) as a substring.
 * https://oeis.org/A225418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 500, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 788 (2026-11-03)
 * Helpers:   is_prime, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A225418.
   Composite numbers that contain the sum of their distinct prime factors
   (sopf) as a decimal substring.
   The function searches composites in increasing order, memoising
   results so any call order works.  Only the first n values
   are generated when a(n) is requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/* Convert a seq_t value to decimal string (no sign).  Assumes out
   large enough (256 bytes is sufficient for all generated terms).   */
static void seq_to_string(seq_t n, char *out)
{
    if (n == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }
    char tmp[256];
    int pos = 0;
    seq_t val = n;
    while (val > 0) {
        uint64_t digit = (uint64_t)(val % 10);
        tmp[pos++] = (char)('0' + digit);
        val = val / 10;
    }
    for (int i = 0; i < pos; ++i)
        out[i] = tmp[pos - 1 - i];
    out[pos] = '\0';
}

/* ----------------------------------------------------------------------
   Auxiliary routines
   ---------------------------------------------------------------------- */

/* Does decimal representation of num contain that of sub?  */
static bool contains_substring(seq_t num, seq_t sub)
{
    char num_buf[256], sub_buf[256];
    seq_to_string(num, num_buf);
    seq_to_string(sub, sub_buf);
    return strstr(num_buf, sub_buf) != NULL;
}

/* Sum of distinct prime factors of a 64‑bit integer.   */
static uint64_t sum_distinct_primes_u64(uint64_t m)
{
    uint64_t sum = 0;
    uint64_t x    = m;
    while (x > 1) {
        uint64_t p = smallest_prime_factor(x);
        sum += p;
        while (x % p == 0) x /= p;
    }
    return sum;
}

/* ----------------------------------------------------------------------
   Sequence cache (memoisation).  The cache grows when more elements
   are requested.  A static pointer remembers the next number to test
   so that repeated calls do not reprobe already examined integers.
   ---------------------------------------------------------------------- */

static seq_t *cache      = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static uint64_t next_candidate = 4; /* start with first composite >3 */

/* Grow the cache capacity to at least <needed> elements.   */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = mem_resize(seq_t, cache, new_cap);
    if (!newp) exit(EXIT_FAILURE); /* allocation failure */
    cache     = newp;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains <goal> elements.  */
static void generate_terms(idx_t goal)
{
    while (cache_len < (size_t)goal) {
        uint64_t cand = next_candidate;
        if (!is_prime(cand)) {
            uint64_t sum = sum_distinct_primes_u64(cand);
            if (contains_substring((seq_t)cand, (seq_t)sum)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
            }
        }
        ++next_candidate;
    }
}

/* ----------------------------------------------------------------------
   Public interface
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset */
    if (cache_len < (size_t)n) generate_terms(n);
    return cache[n - 1];
}
