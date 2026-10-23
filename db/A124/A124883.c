/*
 * A124883  Semiprime triangle, read by rows.
 * https://oeis.org/A124883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth entry of the “semiprime triangle” read by rows
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for the flattened triangle */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored */
static size_t cache_cap = 0;   /* capacity of cache */

/* Flag array: used_flags[i] == 1 iff the natural number i has already
   appeared somewhere in the triangle (except i = 0). */
static uint8_t *used_flags = NULL;
static size_t used_cap = 0;    /* capacity of used_flags */

/* Ensure cache array can hold at least n elements. */
static void ensure_cache_capacity(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure used_flags array can hold indices up to idx (inclusive). */
static void ensure_used_capacity(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    while (new_cap <= idx) new_cap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(used_flags, new_cap * sizeof(uint8_t));
    if (!tmp) exit(EXIT_FAILURE);
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_flags = tmp;
    used_cap = new_cap;
}

/* Test whether n is a semiprime (product of exactly two primes,
   counting multiplicities). n is treated as uint64_t. */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;            /* smallest semiprime is 4 = 2×2 */
    int cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            ++cnt;
            m /= p;
            if (cnt > 2) return false;
        }
    }
    if (m > 1) ++cnt;
    return cnt == 2;
}

/* Global variable holding the length of the last generated row. */
static size_t last_row_len = 0;

/* Generate rows until cache_len >= target (i.e., store at least target
   elements in the flattened cache). */
static void generate_to(size_t target)
{
    if (cache_len == 0) {
        /* First row: T(1,1) = 1 */
        ensure_cache_capacity(1);
        cache[0] = 1;
        cache_len = 1;
        last_row_len = 1;

        ensure_used_capacity(1);
        used_flags[1] = 1;
    }

    while (cache_len < target) {
        size_t new_row_len = last_row_len + 1;          /* next row length */
        ensure_cache_capacity(cache_len + new_row_len);

        /* First element of the new row is always 1 */
        cache[cache_len++] = 1;
        seq_t prev_val = 1;                            /* value to the left */
        ensure_used_capacity(1);
        used_flags[1] = 1;                             /* mark 1 as used */

        for (size_t k = 2; k <= new_row_len; ++k) {
            uint64_t candidate = 2;
            for (;;) {
                ensure_used_capacity(candidate);
                if (!used_flags[candidate]) {
                    /* Check semiprime condition for prev_val + candidate */
                    seq_t sum = prev_val + (seq_t)candidate;
                    uint64_t sum64 = (uint64_t)sum;   /* safe for problem sizes */
                    if (is_semiprime(sum64)) break;
                }
                ++candidate;
            }
            seq_t val = (seq_t)candidate;
            cache[cache_len++] = val;
            ensure_used_capacity(candidate);
            used_flags[candidate] = 1;                 /* mark as used */
            prev_val = val;
        }
        last_row_len = new_row_len;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1 */
    size_t pos = (size_t)(n - 1);
    generate_to(pos + 1);
    return cache[pos];
}
