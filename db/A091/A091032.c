/*
 * A091032  Second column (k=3) of array A090438 ((4,2)-Stirling2) divided by
 *          8.
 * https://oeis.org/A091032
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n)! / 4! for n >= 2 (equivalently a(n) =
 *            A090438(n,3) / 8)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

/* Global cache of primes up to 20000 (enough for n <= 10000) */
static idx_t _prime_cnt = 0;
static idx_t _primes[3000];
static bool _primes_sieved = false;

/* Simple sieve to fill _primes */
static void _sieve_primes(void)
{
    const idx_t LIMIT = 20000;          /* 2 * 10000 */
    bool is_prime[LIMIT + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            _primes[_prime_cnt++] = i;
        }
    }
    _primes_sieved = true;
}

/* Compute base^exp with seq_t arithmetic, using exponentiation by squaring */
static seq_t _pow_seq_t(seq_t base, size_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* value undefined for n < 2 */

    if (!_primes_sieved)
        _sieve_primes();

    /* Compute (n-1) * (2n)! / 24 */
    const idx_t two_n = n * 2U;
    seq_t prod = 1;

    /* Factorial part: multiply prime powers with exponents from (2n)!,
       then subtract exponents of 24 = 2^3 * 3^1. */
    for (size_t i = 0; i < _prime_cnt && _primes[i] <= two_n; ++i) {
        idx_t p = _primes[i];
        idx_t t = two_n / p;
        size_t exp = 0;
        while (t) {
            exp += t;
            t /= p;
        }
        if (p == 2ULL)
            exp -= 3;                 /* remove 2^3 from 24 */
        else if (p == 3ULL)
            exp -= 1;                 /* remove 3^1 from 24 */
        if (exp == 0)
            continue;
        prod *= _pow_seq_t((seq_t)p, exp);
    }

    /* Multiply by (n-1) */
    prod *= (seq_t)(n - 1U);
    return prod;
}
