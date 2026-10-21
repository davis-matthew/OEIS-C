/*
 * A067656  Numbers n such that n!*B(2n) is an integer, where B(2n) are the
 *          Bernoulli numbers.
 * https://oeis.org/A067656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 565, u32 n <= 565, u64 n <= 565,
 *            u128 n <= 565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n!*B(2n) is an integer.
   Equivalently, for every prime p with (p‑1) | 2n, we have p ≤ n.
   The function returns the n‑th such integer (1‑based index). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Sieve globals */
static unsigned char *prime_flag = NULL;   /* sieve flags */
static uint32_t   *prime_list = NULL;     /* list of primes */
static size_t     prime_cnt   = 0;        /* number of primes */
static bool       sieve_ready = false;

/* Simple sieve of Eratosthenes up to LIMIT. */
static void init_sieve(void)
{
    const size_t LIMIT = 200000;           /* enough for indices up to ~10 000 */
    /* Allocate flag array (with cast for C++). */
    prime_flag = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    memset(prime_flag, 1, LIMIT + 1);
    prime_flag[0] = prime_flag[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* Count primes and build list. */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (prime_flag[i])
            ++prime_cnt;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t k = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (prime_flag[i])
            prime_list[k++] = (uint32_t)i;

    sieve_ready = true;
}

/* Test whether a single candidate n satisfies the condition. */
static bool is_valid_n(idx_t n)
{
    uint64_t two_n = 2ULL * (uint64_t)n;   /* works for n up to ~10^9 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p > two_n + 1)        /* primes beyond (2n+1) can be ignored */
            break;
        if (p <= n)                        /* already in n! */
            continue;
        if ((two_n % ((uint64_t)p - 1ULL)) == 0ULL)
            return false;                  /* p would appear in denominator */
    }
    return true;
}

/* Return the n-th term of A067656 (1‑based index). */
seq_t a(idx_t n)
{
    if (!sieve_ready)
        init_sieve();

    size_t found = 0;
    idx_t candidate = 1;

    while (found < (size_t)n) {
        if (is_valid_n(candidate)) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)candidate;    /* cast to seq_t */
        }
        ++candidate;
    }

    /* Should never be reached. */
    return (seq_t)0;
}
