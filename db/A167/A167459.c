/*
 * A167459  Composite numbers in A166504, i.e., whose decimal expansion can be
 *          split up into prime numbers, with leading zeros allowed.
 * https://oeis.org/A167459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big, seq_from_str, seq_realloc, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n-th composite integer whose decimal expansion can be
 * partitioned into prime numbers, allowing leading zeros in the prime parts.
 * The sequence includes only composite members of A166504.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_realloc.h"

/* DP test for whether a number can be split into prime substrings.
 * Leading zeros in parts are treated as the prime value of the parts. */
static bool can_partition(seq_t num)
{
    /* Convert number to its decimal string representation */
    char buf[128];
    size_t len;
    seq_to_str(num, buf, &len);

    /* dp[i] == true if prefix of length i can be partitioned successfully */
    bool dp[128] = {0};
    dp[0] = true;

    for (size_t i = 1; i <= len; ++i) {
        bool ok = false;
        for (size_t j = 0; j < i && !ok; ++j) {
            if (!dp[j]) continue;

            /* Extract substring j..i-1 */
            size_t sublen = i - j;
            if (sublen >= sizeof(buf)) continue; /* safety; should not happen */
            char sub[128];
            memcpy(sub, buf + j, sublen);
            sub[sublen] = '\0';

            seq_t val = seq_from_str(sub);
            if (val >= (seq_t)2 && is_prime_big(val))
                ok = true;
        }
        dp[i] = ok;
    }
    return dp[len];
}

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 4;   /* 2 and 3 are primes, 4 is first composite */

    if (n == 0) return (seq_t)0; /* undefined for n=0, but safe default */

    /* Generate terms until we reach the requested index */
    while (cache_len < n) {
        idx_t cand = next_candidate;

        /* Find next qualifying composite number */
        while (1) {
            seq_t cand_seq = (seq_t)cand;
            if (!is_prime_big(cand_seq) && can_partition(cand_seq))
                break;
            ++cand;
        }

        /* Grow cache if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)cand;
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
