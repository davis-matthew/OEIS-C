/*
 * A101049  Number of partitions of n into parts having at most two
 *          prime-factors.
 * https://oeis.org/A101049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A101049
    a(n) = number of partitions of n into parts having at most two prime factors
    (counting multiplicity).  The parts allowed are 1, all primes,
    all squares of primes and all semiprimes (product of two distinct primes). */

/*  For any idx_t n ≥ 1 this returns a(idx_t) = the partition count.
    The implementation works for seq_t being any unsigned integer type
    (including an arbitrary‑precision class in C++17).  The algorithm
    performs a standard coin‑change DP over the set of allowed parts.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, behaviour for n<1 is irrelevant */

    /* size of arrays: we need indices 0 … n */
    size_t size = (size_t)n + 1ULL;

    /* ----- sieve to find primes up to n ----- */
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    for (size_t i = 2; i < size; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j < size; j += i) is_prime[j] = false;

    /* collect primes into an array */
    uint64_t *primes = (uint64_t *)malloc(size * sizeof(uint64_t));
    size_t prime_cnt = 0;
    for (size_t i = 2; i < size; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint64_t)i;

    /* ----- mark allowed parts ----- */
    bool *allowed = (bool *)calloc(size, sizeof(bool));
    allowed[1] = true;                         /* 1 has 0 prime factors */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p <= (uint64_t)n) allowed[(size_t)p] = true;
        if (p * p <= (uint64_t)n) allowed[(size_t)(p * p)] = true;
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t prod = p * q;
            if (prod > (uint64_t)n) break;
            allowed[(size_t)prod] = true;
        }
    }

    /* ----- unbounded coin change DP ----- */
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    dp[0] = 1;                                 /* empty partition */

    for (size_t part = 1; part < size; ++part) {
        if (!allowed[part]) continue;
        for (size_t j = part; j < size; ++j) {
            dp[j] = dp[j] + dp[j - part];
        }
    }

    seq_t result = dp[n];

    /* ----- cleanup ----- */
    free(is_prime);
    free(primes);
    free(allowed);
    free(dp);

    return result;
}
