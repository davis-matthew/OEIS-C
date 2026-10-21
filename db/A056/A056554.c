/*
 * A056554  Powerfree kernel of 4th-powerfree part of n.
 * https://oeis.org/A056554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of all prime divisors of n whose exponent modulo
 *            4 is non‑zero (i.e. a(n) = rad of the 4th‑powerfree part of n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* generate primes up to 1,000,000 */
#define MAXP 1000000
static uint32_t _primes[80000];
static size_t _prime_cnt = 0;
static int _primes_inited = 0;

static void _init_primes(void)
{
    if (_primes_inited) return;
    static uint8_t sieve[MAXP + 1];
    memset(sieve, 1, MAXP + 1);
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint32_t)i * i <= MAXP; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= MAXP; j += i) sieve[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= MAXP; ++i) {
        if (sieve[i]) _primes[_prime_cnt++] = i;
    }
    _primes_inited = 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;             /* a(1) = 1, a(0) is unused */

    _init_primes();

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (size_t i = 0; i < _prime_cnt && (idx_t)_primes[i] * (idx_t)_primes[i] <= temp; ++i) {
        uint32_t p = _primes[i];
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 3U) {            /* exp % 4 != 0 */
                result *= (seq_t)p;
            }
        }
    }

    if (temp > 1) {                    /* remaining prime factor */
        result *= (seq_t)temp;          /* exponent is 1 -> not divisible by 4 */
    }

    return result;
}
