/*
 * A054008  n read modulo (number of divisors of n).
 * https://oeis.org/A054008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod tau(n) tau(n) = number of divisors of n
 * Range:     u8 n <= 331, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple sieve of Eratosthenes up to 1 000 000 to factor n */
/* and compute tau(n) in O(π(√n)) time.                                      */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint32_t * _prime_tbl = NULL;
static int _prime_cnt = 0;

/* Build a table of primes ≤ 1 000 000 (≈78498 primes).  Safety limit for n up to ~10¹². */
static void build_prime_table(void)
{
    const uint32_t LIMIT = 1000000U;
    if (_prime_cnt) return;            /* already built */

    uint8_t *is_comp = (uint8_t*)calloc(LIMIT + 1, 1);
    if (!is_comp) return;              /* allocation failure – unlikely */

    uint32_t *primes = (uint32_t*)malloc((LIMIT / 10 + 10) * sizeof(uint32_t));
    if (!primes) return;

    int pc = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    _prime_tbl = primes;
    _prime_cnt = pc;
    free(is_comp);
}

/* Compute τ(n) for n ≥ 1.  Uses the prebuilt prime table.            */
static uint64_t divisor_count(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t nt = n;
    uint64_t tau = 1;
    for (int i = 0; i < _prime_cnt; ++i) {
        uint32_t p = _prime_tbl[i];
        if ((uint64_t)p * p > nt) break;
        if (nt % p == 0) {
            int exp = 0;
            while (nt % p == 0) {
                nt /= p;
                ++exp;
            }
            tau *= (uint64_t)(exp + 1);
        }
    }
    if (nt > 1)       /* remaining prime factor */
        tau *= 2;
    return tau;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – sequence defined for n ≥ 1 */
    if (!_prime_cnt) build_prime_table();
    uint64_t tau = divisor_count((uint64_t)n);
    return (seq_t)n % (seq_t)tau;
}
