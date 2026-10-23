/*
 * A175416  Those positive integers n that when written in binary, each run of
 *          0's and 1's has a length which is coprime to the number of binary
 *          digits of n.
 * https://oeis.org/A175416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose binary representation has every run length
   coprime to its bit length.  Compute by brute‑force search and cache results.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 0;

static bool good_candidate(idx_t x)
{
    /* Determine bit length */
    unsigned int l = 0;
    uint64_t tmp = (uint64_t)x;
    while (tmp) {
        l++;
        tmp >>= 1;
    }
    if (l == 0) return false;

    /* Scan binary runs from most significant bit to least */
    unsigned int run_len = 1;
    int prev_bit = (int)((x >> (l - 1)) & 1ULL);

    for (int pos = (int)l - 2; pos >= 0; --pos) {
        int cur_bit = (int)((x >> pos) & 1ULL);
        if (cur_bit == prev_bit) {
            run_len++;
        } else {
            if (gcd((seq_t)run_len, (seq_t)l) != (seq_t)1)
                return false;
            prev_bit = cur_bit;
            run_len = 1;
        }
    }
    /* Final run */
    if (gcd((seq_t)run_len, (seq_t)l) != (seq_t)1)
        return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_checked = 0;
    }

    while (cache_len < (size_t)n) {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        if (good_candidate(cand)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
