/*
 * A079018  Suppose p and q = p+16 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 17 possible difference patterns, namely
 *          [16], [4,12], [6,10], [10,6], [12,4], [4,2,10], [4,6,6], [4,8,4],
 *          [6,4,6], [6,6,4], [10,2,4], [4,2,4,6],...
 * https://oeis.org/A079018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A079018 by enumerating prime pairs p,p+16, extracting the
   difference pattern of primes between them, storing the smallest p
   producing each pattern, and returning the nth smallest such p
   (indexed from 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

struct Pattern {
    int *diff;          /* the difference sequence */
    size_t len;         /* number of differences */
    uint64_t minimal_p; /* smallest p that produced this pattern */
};

static bool initialized = false;
static seq_t seq[17] = {0};

static void init_sequence(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Sieve limit – far larger than the largest known minimal p (1831) */
    const idx_t LIMIT = 200000;
    /* Allocate storage for the primality test array */
    char *is_prime = (char *)malloc((size_t)(LIMIT + 1));
    if (!is_prime)
        return;
    memset(is_prime, 1, (size_t)(LIMIT + 1));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Collect primes up to LIMIT */
    uint64_t *primes = (uint64_t *)malloc((size_t)(LIMIT / 10) * sizeof(uint64_t)); /* oversize */
    if (!primes) {
        free(is_prime);
        return;
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i])
            primes[prime_cnt++] = (uint64_t)i;
    }

    /* Search prime pairs p, p+16 and collect patterns */
    struct Pattern patterns[17];
    size_t pattern_cnt = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = (idx_t)primes[idx];
        if (p + 16 > LIMIT)
            break;
        if (!is_prime[p + 16])
            continue; /* skip, not a pair of primes */

        /* Build the difference sequence for primes in [p, p+16] */
        int diffs[17];
        size_t len = 0;
        int prev = -1;
        for (idx_t x = p; x <= p + 16; ++x) {
            if (is_prime[x]) {
                if (prev >= 0)
                    diffs[len++] = (int)(x - prev);
                prev = (int)x;
            }
        }

        /* Look for an existing pattern */
        size_t found = pattern_cnt;
        for (size_t i = 0; i < pattern_cnt; ++i) {
            if (patterns[i].len != len)
                continue;
            bool equal = true;
            for (size_t k = 0; k < len; ++k) {
                if (patterns[i].diff[k] != diffs[k]) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                found = i;
                break;
            }
        }

        if (found < pattern_cnt) {
            /* Existing pattern – keep the smaller p */
            if (p < patterns[found].minimal_p)
                patterns[found].minimal_p = p;
        } else {
            /* New pattern */
            patterns[pattern_cnt].len = len;
            patterns[pattern_cnt].diff = (int *)malloc(len * sizeof(int));
            memcpy(patterns[pattern_cnt].diff, diffs, len * sizeof(int));
            patterns[pattern_cnt].minimal_p = p;
            ++pattern_cnt;
        }
    }

    /* Free temporary allocations */
    free(is_prime);
    free(primes);

    /* Sort patterns by their minimal_p */
    if (pattern_cnt > 1) {
        /* Simple insertion sort for <=17 elements */
        for (size_t i = 1; i < pattern_cnt; ++i) {
            struct Pattern key = patterns[i];
            size_t j = i;
            while (j > 0 && patterns[j - 1].minimal_p > key.minimal_p) {
                patterns[j] = patterns[j - 1];
                --j;
            }
            patterns[j] = key;
        }
    }

    /* Store sorted minimal_p values into seq[] */
    for (size_t i = 0; i < pattern_cnt && i < 17; ++i) {
        seq[i] = (seq_t)patterns[i].minimal_p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    init_sequence();
    if (n <= 17)
        return seq[n - 1];
    return (seq_t)0;
}
