/*
 * A123300  Prime sums of 14 positive 5th powers.
 * https://oeis.org/A123300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   the sequence consists of primes that can be written as the sum
 *            of exactly 14 positive fifth powers. The function a(n) returns
 *            the n-th such prime
 * Range:     u8 n <= 1, u16 n <= 2265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: raise an integer to the fifth power (fits in 64 bits
   for the limits we need). */
static uint64_t pow5_uint64(uint64_t x)
{
    uint64_t r = 1;
    for (int i = 0; i < 5; ++i) r *= x;
    return r;
}

/* Helper: shift a bitset left by 'shift' bits and OR into dest. */
static void shift_or(uint64_t *dest, const uint64_t *src,
                     size_t words, size_t shift)
{
    size_t wshift   = shift >> 6;        /* words to shift */
    unsigned bshift = shift & 63;        /* bits to shift within word */

    if (bshift == 0) {
        for (size_t i = words; i-- > wshift; ) {
            dest[i] |= src[i - wshift];
        }
    } else {
        for (size_t i = words; i-- > wshift; ) {
            if (i == wshift) {
                dest[i] |= src[0] << bshift;
            } else {
                dest[i] |= (src[i - wshift] << bshift) |
                           (src[i - wshift - 1] >> (64 - bshift));
            }
        }
    }
}

/* Static state to cache computed primes. */
static seq_t *prime_list = NULL;   /* array of primes that are sums of 14 fifth powers */
static size_t prime_list_len = 0;  /* number of elements in prime_list   */
static uint64_t computed_limit = 0;/* limit up to which we have computed */

static void rebuild_cache(uint64_t new_limit)
{
    /* --------- Build bitset for sums of 14 fifth powers --------- */
    size_t words = (new_limit >> 6) + 1;
    uint64_t **dp = (uint64_t **)malloc(15 * sizeof(uint64_t *));
    for (int k = 0; k <= 14; ++k)
        dp[k] = (uint64_t *)calloc(words, sizeof(uint64_t));

    dp[0][0] = 1ULL;                /* sum 0 with 0 terms */

    /* Pre‑compute all fifth powers that do not exceed new_limit */
    uint64_t *powers = NULL;
    size_t powers_cnt = 0;
    for (uint64_t i = 1; ; ++i) {
        uint64_t p = pow5_uint64(i);
        if (p > new_limit) break;
        powers = (uint64_t *)realloc(powers, (powers_cnt + 1) * sizeof(uint64_t));
        powers[powers_cnt++] = p;
    }

    /* Add each power one by one, updating dp[1]..dp[14] */
    for (size_t idx = 0; idx < powers_cnt; ++idx) {
        uint64_t p = powers[idx];
        for (int k = 1; k <= 14; ++k)
            shift_or(dp[k], dp[k - 1], words, p);
    }

    /* --------- Sieve of Eratosthenes --------- */
    size_t sieve_size = new_limit + 1;
    uint8_t *is_prime = (uint8_t *)calloc(sieve_size, sizeof(uint8_t));
    if (!is_prime) return;
    for (size_t i = 2; i < sieve_size; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* --------- Collect all prime sums --------- */
    /* First count how many we will store. */
    size_t count = 0;
    for (size_t n = 2; n <= new_limit; ++n) {
        if (!is_prime[n]) continue;
        if ((dp[14][n >> 6] >> (n & 63)) & 1)
            ++count;
    }

    seq_t *new_list = (seq_t *)malloc(count * sizeof(seq_t));
    size_t pos = 0;
    for (size_t n = 2; n <= new_limit; ++n) {
        if (!is_prime[n]) continue;
        if ((dp[14][n >> 6] >> (n & 63)) & 1) {
            new_list[pos++] = (seq_t)n;
        }
    }

    /* Free old resources */
    for (int k = 0; k <= 14; ++k) free(dp[k]);
    free(dp);
    free(powers);
    free(is_prime);

    /* Replace global list */
    free(prime_list);
    prime_list = new_list;
    prime_list_len = count;
    computed_limit = new_limit;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, return 0 */

    /* Ensure we have enough terms computed. */
    while ((idx_t)prime_list_len < n) {
        uint64_t new_limit = (computed_limit == 0) ? 200000
                                : computed_limit * 2;
        rebuild_cache(new_limit);
    }
    return prime_list[n - 1];
}
