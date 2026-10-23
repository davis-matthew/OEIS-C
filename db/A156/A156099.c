/*
 * A156099  Smallest number m such that prime(n) is a factor of both m and
 *          sigma(m).
 * https://oeis.org/A156099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that prime(n) divides both m and σ(m)
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 1237, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* algorithm: find the smallest t coprime to p where σ(t) ≡ 0 (mod p);
   then m = p * t.  Here σ denotes the sum–of–divisors function.     */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external build types */
extern seq_t a(idx_t n);          /* declared below */

/* utilities from the build environment */
#include "../utils/get_prime.h"    /* get_prime(idx_t n) → uint32_t */

/* --------------------------------------------------------------------- */
/* Prime sieve – static data                                                      */
/* --------------------------------------------------------------------- */
static uint32_t *prime_table      = NULL;
static size_t   prime_count       = 0;
static size_t   prime_table_cap   = 0;
static bool     prime_table_ready = false;
#define PRIME_LIMIT 1000000        /* sufficient for all test cases */

/* create a simple sieve of Eratosthenes up to PRIME_LIMIT   */
static void init_primes(void)
{
    if (prime_table_ready) return;

    /* allocate enough space for the primes (≈78k for 1e6) */
    prime_table_cap = 80000;
    prime_table = (uint32_t *)malloc(prime_table_cap * sizeof(uint32_t));
    if (!prime_table) exit(1);

    bool *composite = (bool *)calloc(PRIME_LIMIT + 1, 1);
    if (!composite) exit(1);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            if (prime_count == prime_table_cap) {
                prime_table_cap <<= 1;
                prime_table = (uint32_t *)realloc(prime_table, prime_table_cap * sizeof(uint32_t));
                if (!prime_table) exit(1);
            }
            prime_table[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
    prime_table_ready = true;
}

/* --------------------------------------------------------------------- */
/*  σ(t) modulo p  (p is prime).  Compute directly via factorisation          */
/* --------------------------------------------------------------------- */
static uint32_t sigma_mod(uint64_t t, uint32_t p)
{
    uint32_t result = 1;
    uint64_t remain = t;

    for (size_t idx = 0; idx < prime_count && (uint64_t)prime_table[idx] * prime_table[idx] <= remain; ++idx) {
        uint32_t q = prime_table[idx];
        if (remain % q == 0) {
            int exp = 0;
            while (remain % q == 0) {
                remain /= q;
                ++exp;
            }
            /* compute 1 + q + q² + … + q^exp  (mod p) */
            uint32_t sum  = 1;
            uint32_t power = 1;
            for (int i = 1; i <= exp; ++i) {
                power = (uint32_t)(( (uint64_t)power * q ) % p);
                sum   = (uint32_t)((sum + power) % p);
            }
            result = (uint32_t)(( (uint64_t)result * sum ) % p);
        }
    }
    /* remaining prime factor > sqrt(t)  */
    if (remain > 1) {
        uint32_t q = (uint32_t)remain;
        uint32_t sum  = 1;
        uint32_t power = 1;
        /* exponent is 1 */
        power = (uint32_t)(( (uint64_t)power * q ) % p);
        sum   = (uint32_t)((sum + power) % p);
        result = (uint32_t)(( (uint64_t)result * sum ) % p);
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 0 */

    /* ensure the prime table and get the n-th prime */
    init_primes();
    uint32_t p = get_prime(n);
    if (p == 0) return (seq_t)0;  /* safety fallback */

    /* search for the smallest t coprime to p such that σ(t) ≡ 0 (mod p) */
    uint64_t t = 2;
    for (;;) {
        if (t % p == 0) {          /* ensure gcd(t,p)=1 */
            ++t;
            continue;
        }
        if (sigma_mod(t, p) == 0) {
            /* m = p * t */
            return (seq_t)p * (seq_t)t;
        }
        ++t;
    }
}
