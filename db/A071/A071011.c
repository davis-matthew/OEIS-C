/*
 * A071011  Numbers n such that n is a sum of 2 squares (i.e., n is in
 *          A001481(k)) and sigma(n) == 0 (mod 4).
 * https://oeis.org/A071011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 6226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the OEIS A071011 sequence by scanning integers
   and accepting those that are both sums of two squares and whose
   divisor-sum σ(n) is ≡ 0 (mod 4).  The sequence is built on demand
   and cached so that arbitrary calling order is supported. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool good_candidate(uint64_t n)
{
    uint64_t m = n;
    int sigma_mod4 = 1;   /* product of (sum_{k=0..e} p^k) modulo 4 */

    /* Handle factor 2 first */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        /* compute sum_{k=0..e} 2^k modulo 4 */
        int sum_mod4 = 1;      /* 2^0 = 1 */
        int pow_mod4 = 1;      /* current power of 2 modulo 4 */
        for (uint64_t i = 1; i <= e; ++i) {
            pow_mod4 = (pow_mod4 * 2) & 3;
            sum_mod4 = (sum_mod4 + pow_mod4) & 3;
        }
        sigma_mod4 = (sigma_mod4 * sum_mod4) & 3;
    }

    /* Handle odd prime factors */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d != 0)
            continue;
        uint64_t e = 0;
        while (m % d == 0) {
            m /= d;
            ++e;
        }
        /* sum of two squares condition */
        if ((d % 4 == 3) && (e & 1))
            return false;   /* fails sum-of-two-squares test */

        /* compute sum_{k=0..e} d^k modulo 4 */
        int p4 = d % 4;
        int sum_mod4 = 1;
        int pow_mod4 = 1;
        for (uint64_t i = 1; i <= e; ++i) {
            pow_mod4 = (pow_mod4 * p4) & 3;
            sum_mod4 = (sum_mod4 + pow_mod4) & 3;
        }
        sigma_mod4 = (sigma_mod4 * sum_mod4) & 3;
    }

    /* If a prime factor > sqrt(m) remains */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;   /* exponent is 1 */
        if ((p % 4 == 3) && (e & 1))
            return false;   /* fails sum-of-two-squares test */

        /* compute sum_{k=0..e} p^k modulo 4 */
        int p4 = p % 4;
        int sum_mod4 = 1;
        int pow_mod4 = 1;
        for (uint64_t i = 1; i <= e; ++i) {
            pow_mod4 = (pow_mod4 * p4) & 3;
            sum_mod4 = (sum_mod4 + pow_mod4) & 3;
        }
        sigma_mod4 = (sigma_mod4 * sum_mod4) & 3;
    }

    return (sigma_mod4 == 0);
}

/* Static cache for the generated sequence */
static seq_t *cache      = NULL;
static size_t cache_cnt  = 0;      /* number of terms cached   */
static size_t cache_cap  = 0;      /* allocated capacity       */
static uint64_t next_candidate = 1; /* next integer to test    */

static void cache_append(seq_t val)
{
    if (cache_cnt == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* Generate terms until we reach the requested index */
    while (cache_cnt < (size_t)n) {
        if (good_candidate(next_candidate))
            cache_append((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
