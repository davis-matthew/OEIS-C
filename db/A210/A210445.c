/*
 * A210445  Least positive integer k with k*n practical.
 * https://oeis.org/A210445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 750, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least positive integer k such that k*n is a practical number.
   The function iterates k = 1,2,… and tests each product for practicality
   using the known characterization:
   a number is practical iff its prime factors p_1 < p_2 < … satisfy
   p_1 = 2 and, for i > 1, p_i ≤ 1 + σ(∏_{j<i} p_j^{e_j}) ,
   where σ is the sum‑of‑divisors function. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* We generate a list of primes once; it is large enough for all testable
   values (sufficient for n up to at least 10^12). */
#define PRIME_LIMIT 50000          /* √(PRIME_LIMIT^2) ≈ 5×10^4 >> 10^8 */
#define MAX_PRIMES  5133           /* number of primes <= 50000 */

static uint64_t _primes[MAX_PRIMES];
static size_t   _prime_cnt = 0;
static bool     _primes_ready = false;

/* Simple sieve to fill the _primes array; called lazily on first use. */
static void _init_primes(void)
{
    if (_primes_ready) return;
    _primes_ready = true;
    /* Sieve of Eratosthenes up to PRIME_LIMIT. */
    bool small[PRIME_LIMIT + 1] = { false };
    size_t i, j;
    for (i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (!small[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i)
                small[j] = true;
    }
    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (!small[i]) _primes[_prime_cnt++] = i;
}

/* Test whether m is a practical number.  m is assumed to fit in uint64_t. */
static bool _is_practical(uint64_t m)
{
    if (m <= 1) return true;

    _init_primes();

    /* Factor m; store primes and exponents in ascending order. */
    uint64_t p[64];
    uint8_t e[64];
    size_t pcnt = 0;
    uint64_t temp = m;
    for (size_t i = 0; i < _prime_cnt && _primes[i] * _primes[i] <= temp; ++i)
    {
        if (temp % _primes[i] == 0) {
            uint8_t cnt = 0;
            do {
                temp /= _primes[i];
                ++cnt;
            } while (temp % _primes[i] == 0);
            p[pcnt] = _primes[i];
            e[pcnt] = cnt;
            ++pcnt;
        }
    }
    if (temp > 1) {
        p[pcnt] = temp;
        e[pcnt] = 1;
        ++pcnt;
    }

    if (pcnt == 0 || p[0] != 2) return false;

    uint64_t sigma = 1;  /* σ of the processed part (initially 1). */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t prime = p[i];
        if (prime > sigma + 1) return false;

        /* Compute σ(p^e). */
        uint64_t term = 1;
        uint64_t power = 1;
        for (uint8_t k = 0; k < e[i]; ++k) {
            power *= prime;
            term += power;
        }
        sigma *= term;
    }
    return true;
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    /* Search for the minimal k. m = k*n may grow beyond 32 bits, but
       stays comfortably within 64 bits for the intended use. */
    uint64_t k = 1;
    for (;; ++k) {
        uint64_t m = n * k;
        /* Guard against overflow: if n*k overflows uint64_t, stop. */
        if (m / k != n) break;
        if (_is_practical(m)) return (seq_t)k;
    }
    /* Fallback: should not happen for the defined domain. */
    return (seq_t)1;
}
