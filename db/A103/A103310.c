/*
 * A103310  Largest prime primitive root of n that is less than n, or 0 if
 *          none exists.
 * https://oeis.org/A103310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file implements the OEIS A103310 sequence:
   a(n) = largest prime primitive root of n that is < n, or 0 if none exists.
   The algorithm:
   1. Checks that n has a primitive root (only for n=1,2,4,p^k,2*p^k with odd prime p).
   2. Computes φ(n) and its distinct prime factors.
   3. Scans the prime list from largest to smallest,
      skipping primes that are not coprime to n, and tests the
      primitive‑root criterion: for every prime divisor q of φ(n)
      a^(φ(n)/q) mod n ≠ 1.
   4. Returns the first prime that satisfies the test, or 0. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/
/* Pre‑computed primes up to 200 000 – more than enough for n ≤ 10 000.     */
/*----------------------------------------------------------------------------*/
#define PRIME_LIMIT 200000
static uint64_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[prime_count++] = i;
    free(is_comp);
    primes_initialized = true;
}

/*----------------------------------------------------------------------------*/
/* Modular exponentiation (base^exp mod mod).                                 */
/*----------------------------------------------------------------------------*/
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) res = (unsigned __int128)res * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

/*----------------------------------------------------------------------------*/
/* Euler’s totient function φ(n) using trial division.                        */
/*----------------------------------------------------------------------------*/
static uint64_t phi_value(uint64_t n)
{
    uint64_t result = n, temp = n;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/*----------------------------------------------------------------------------*/
/* Number of distinct odd prime factors and exponent of 2 in n.                */
/* Used to test whether primitive roots exist.                                 */
/*----------------------------------------------------------------------------*/
static bool has_primitive_root(uint64_t n)
{
    if (n == 2 || n == 4) return true;
    uint64_t temp = n;
    unsigned exp2 = 0;
    unsigned odd_primes = 0;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (p == 2) exp2 = e;
            else odd_primes++;
        }
    }
    if (temp > 1) {
        if (temp == 2) exp2 = 1;      /* remaining factor is 2 */
        else odd_primes++;            /* remaining factor is an odd prime */
    }
    /* Primitive roots exist only for n = p^k or 2*p^k, with odd prime p. */
    if (odd_primes == 1 && exp2 == 0) return true;   /* p^k */
    if (odd_primes == 1 && exp2 == 1) return true;   /* 2*p^k */
    return false;
}

/*----------------------------------------------------------------------------*/
/* Factorization of a uint64_t into distinct primes (up to a limit).           */
/* The factor array receives distinct prime factors (at most 20 here).           */
/*----------------------------------------------------------------------------*/
static size_t factor_uint64(uint64_t n, uint64_t *factors,
                            unsigned *dummy, size_t max_factors)
{
    size_t count = 0;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            if (count < max_factors) {
                factors[count] = p;
                dummy[count] = e;
            }
            count++;
        }
    }
    if (n > 1 && count < max_factors) {
        factors[count] = n;
        dummy[count] = 1;
        count++;
    }
    return count;
}

/*----------------------------------------------------------------------------*/
/* Main sequence function.                                                   */
/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init_primes();

    uint64_t N = (uint64_t)n;
    if (N < 3) return 0;

    if (!has_primitive_root(N)) return 0;

    uint64_t phiN = phi_value(N);

    /* Collect distinct prime factors of φ(N). */
    uint64_t phi_factors[20];
    unsigned dummy[20];
    size_t phi_factor_count = factor_uint64(phiN, phi_factors, dummy, 20);

    /* Search for largest prime < N that is a primitive root. */
    for (size_t i = prime_count; i-- > 0;) {
        uint64_t p = primes[i];
        if (p >= N) continue;          /* must be strictly less than N */
        if (N % p == 0) continue;      /* not coprime → cannot be a root */

        bool ok = true;
        for (size_t j = 0; j < phi_factor_count; ++j) {
            uint64_t d = phi_factors[j];
            if (modpow_uint64(p, phiN / d, N) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)p;
    }
    return 0;
}
