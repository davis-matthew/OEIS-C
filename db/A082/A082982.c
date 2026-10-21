/*
 * A082982  Numbers k such that k, k+1 and k+2 are sums of 2 squares.
 * https://oeis.org/A082982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th integer k such that k, k+1 and k+2 are each sums of
 * two integer squares.  All such k are multiples of 8.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Prime sieve for trial division – enough for n up to ~10^5 (k ~ 8·10^5). */
#define PRIME_MAX 100000       /* covers the needed range for 64‑bit types      */
static uint32_t primes[PRIME_MAX];
static size_t prime_cnt = 0;
static bool primes_done = false;

static void init_primes(void)
{
    if (primes_done) return;
    bool *sieve = (bool *)malloc(PRIME_MAX + 1);
    if (!sieve) return;                      /* ulimit error – unlikely        */
    memset(sieve, true, PRIME_MAX + 1);
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_MAX; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                sieve[j] = false;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
    free(sieve);
    primes_done = true;
}

/* --------------------------------------------------------------------- */
/* Test whether a given unsigned integer is a sum of two squares.      */
/* Uses the classical characterization: p ≡ 3 (mod 4) must appear with
 * an even exponent in the prime factorization.                          */
static bool is_sum_two_squares(uint64_t n)
{
    if (n == 0) return true;                     /* 0 = 0² + 0² */
    uint64_t m = n;
    init_primes();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > m) break;
        if (m % p == 0) {
            int exp = 0;
            do { m /= p; ++exp; } while (m % p == 0);
            if ((p % 4) == 3 && (exp & 1)) return false;
        }
    }
    /* Remaining factor (if any) is prime and > sqrt(original n).          */
    if (m > 1 && (m % 4) == 3) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/* Main sequence generator.  This function is stateless and can be called
 * in any order.                                                    */
seq_t a(idx_t n)
{
    /* Search over multiples of 8 only, since all terms are divisible by 8. */
    uint64_t count = 0;
    for (uint64_t j = 0; ; ++j) {
        uint64_t k = 8ULL * j;
        if (is_sum_two_squares(k) &&
            is_sum_two_squares(k + 1) &&
            is_sum_two_squares(k + 2)) {
            ++count;
            if (count == n) return (seq_t)k; /* construction from 64‑bit value */
        }
    }
}
