/*
 * A136446  Numbers k such that some subset of the numbers { 1 < d < k : d
 *          divides k } adds up to k.
 * https://oeis.org/A136446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 9648, u32 n <= 9648, u64 n <= 9648,
 *            u128 n <= 9648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonn: numbers k such that some subset of {1 < d < k : d | k} sums to k
   Formula: a(n) is the n‑th such integer (a(1)=12). This implementation
   tests each integer sequentially using a subset‑sum bitset DP. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Return true iff k has a subset of its proper divisors >1 that sums to k. */
static bool has_subset_sum(uint64_t k)
{
    /* collect proper divisors >1 and <k */
    size_t div_cap = 0;
    uint64_t *divs = NULL;
    size_t divcnt = 0;

    for (uint64_t i = 2; i <= k / i; ++i) {
        if (k % i == 0) {
            if (divcnt >= div_cap) {
                size_t new_cap = div_cap ? div_cap * 2 : 8;
                divs = (uint64_t *)realloc(divs, new_cap * sizeof(uint64_t));
                div_cap = new_cap;
            }
            divs[divcnt++] = i;
            uint64_t j = k / i;
            if (j != i && j != k) {
                if (divcnt >= div_cap) {
                    size_t new_cap = div_cap ? div_cap * 2 : 8;
                    divs = (uint64_t *)realloc(divs, new_cap * sizeof(uint64_t));
                    div_cap = new_cap;
                }
                divs[divcnt++] = j;
            }
        }
    }

    if (divcnt == 0) {
        free(divs);
        return false;          /* only divisor is 1 */
    }

    /* quick check: sum of all divisors must be at least k */
    uint64_t total = 0;
    for (size_t i = 0; i < divcnt; ++i) total += divs[i];
    if (total < k) {
        free(divs);
        return false;
    }

    /* bitset DP: one bit for each sum 0..k */
    size_t words = (k >> 6) + 1;
    uint64_t *dp = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!dp) { free(divs); return false; }      /* allocation failure -> skip */
    memset(dp, 0, words * sizeof(uint64_t));
    dp[0] = 1ULL;                               /* sum 0 is always possible */

    for (size_t idx = 0; idx < divcnt; ++idx) {
        uint64_t d = divs[idx];
        size_t d_word = d >> 6;
        unsigned d_bit  = (unsigned)(d & 63U);

        for (size_t i = words; i-- > d_word; ) {
            uint64_t shifted = dp[i - d_word] << d_bit;
            if (d_bit && i > d_word)
                shifted |= dp[i - d_word - 1] >> (64 - d_bit);
            dp[i] |= shifted;
        }

        size_t target_w = k >> 6;
        unsigned target_b = (unsigned)(k & 63U);
        if (dp[target_w] & (1ULL << target_b)) {
            free(divs);
            free(dp);
            return true;      /* found subset sum equal to k */
        }
    }

    free(divs);
    free(dp);
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cached = 0;
    static uint64_t next_k = 2;   /* smallest integer to test */

    /* extend cache array if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* generate terms until we have n terms */
    while (cached < (size_t)n) {
        if (has_subset_sum(next_k))
            cache[cached++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
