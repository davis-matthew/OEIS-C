/*
 * A009003  Hypotenuse numbers (squares are sums of 2 nonzero squares).
 * https://oeis.org/A009003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th positive integer that has a prime divisor
   ≡ 1 (mod 4).  The function generates the numbers in increasing order
   using a sieve of small prime factors and collects those that possess
   such a prime factor.  The results are memoized so that calls with
   arbitrary n are handled efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_data = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;    /* next integer to test */

static uint32_t *spf = NULL;          /* smallest prime factor array */
static uint64_t spf_limit = 0;

/* Allocate and compute spf array up to new_limit.  All entries fit in uint32_t. */
static void recompute_spf(uint64_t new_limit)
{
    free(spf);
    spf = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= new_limit; ++i)
        spf[i] = 0;

    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= new_limit) {
                for (uint64_t j = i * i; j <= new_limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = new_limit;
}

/* Test whether x has a prime divisor ≡ 1 (mod 4).  Assumes spf_limit ≥ x. */
static bool has_prime1mod4(uint64_t x)
{
    if (x <= 1)
        return false;
    while (x > 1) {
        uint32_t p = spf[x];
        if (p % 4 == 1)
            return true;
        /* Skip all copies of the same prime factor. */
        uint64_t pp = p;
        while (x % pp == 0)
            x /= pp;
    }
    return false;
}

/* Append a value to the cached sequence array, growing as necessary. */
static void push_seq(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? (cache_cap << 1) : 16;
        seq_data = (seq_t*)realloc(seq_data, cache_cap * sizeof(seq_t));
    }
    seq_data[cache_len++] = val;
}

/* Ensure that seq_data contains at least n entries. */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;
    while (cache_len < (size_t)n) {
        /* Make sure spf array covers next_candidate. */
        if (spf_limit < next_candidate) {
            uint64_t new_limit = next_candidate * 2 + 100;
            if (new_limit < 16)
                new_limit = 16;
            recompute_spf(new_limit);
        }

        /* Scan numbers until we gather enough terms. */
        while (next_candidate <= spf_limit && cache_len < (size_t)n) {
            if (has_prime1mod4(next_candidate))
                push_seq((seq_t)next_candidate);
            ++next_candidate;
        }

        /* If still insufficient, extend the sieve limit. */
        if (cache_len < (size_t)n) {
            uint64_t new_limit = spf_limit * 2 + 100;
            if (new_limit < next_candidate * 2)
                new_limit = next_candidate * 2;
            recompute_spf(new_limit);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; undefined for n = 0 */
    ensure_cache(n);
    return seq_data[(size_t)n - 1];
}
