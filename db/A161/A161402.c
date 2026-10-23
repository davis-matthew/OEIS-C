/*
 * A161402  Numbers having more than 2 primes among the permutations of their
 *          digits.
 * https://oeis.org/A161402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A161402: numbers having more than 2 primes among
   permutations of their decimal digits (leading zeros ignored). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Recursive helper to count primes among digit permutations */
static void permute_rec(int pos, int len, int counts[10],
                        unsigned long long current, int *prime_cnt)
{
    if (*prime_cnt > 2) return;
    if (pos == len) {
        if (is_prime_big((seq_t)current)) (*prime_cnt)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (counts[d] > 0) {
            --counts[d];
            permute_rec(pos + 1, len, counts, current * 10 + d, prime_cnt);
            ++counts[d];
            if (*prime_cnt > 2) return;
        }
    }
}

/* Determine whether n has more than two prime permutations of its digits */
static bool qualifies(seq_t n)
{
    /* Extract digits and counts */
    unsigned long long val = (unsigned long long)n;
    int counts[10] = {0};
    int len = 0;
    if (val == 0) return false;
    while (val > 0) {
        ++counts[val % 10];
        val /= 10;
        ++len;
    }

    int prime_cnt = 0;
    permute_rec(0, len, counts, 0ULL, &prime_cnt);
    return prime_cnt > 2;
}

/* Static cache for computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t max_checked = 99;   /* last integer tested */

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe return */

    /* initialize cache on first call */
    if (cache == NULL) {
        ensure_capacity(1024, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache_size = 0;
        max_checked = 99;
    }

    /* generate terms until we have n items */
    seq_t cand = (seq_t)(max_checked + 1);
    while (cache_size < n) {
        if (qualifies(cand)) {
            ensure_capacity(cache_size + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = cand;
        }
        ++cand;
    }
    max_checked = (idx_t)cand - 1;

    return cache[n - 1];
}
