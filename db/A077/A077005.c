/*
 * A077005  Smallest k such that prime(n) divides k*prime(n-1) + 1, n > 1.
 * https://oeis.org/A077005
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that prime(n) divides k*prime(n-1)+1, n >
 *            1. Implementation uses the congruence k*q ≡ -1 (mod p), where
 *            p=prime(n), q=prime(n-1). Since q has an inverse modulo the
 *            prime p, k ≡ (p-1)*q⁻¹ (mod p). The modular inverse is computed
 *            by Fermat's little theorem: q⁻¹ ≡ q^(p-2) (mod p)
 * Range:     u8 n <= 67, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- prime generation ------------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* array of primes  */
static size_t   prime_count = 0;      /* number of primes stored   */
static size_t   prime_capacity = 0;   /* allocated capacity       */
static uint64_t next_candidate = 2;   /* next number to test */

/* Ensure that at least `limit` primes are stored. */
static void ensure_primes(idx_t limit)
{
    while (prime_count < (size_t)limit)
    {
        /* Find next prime starting at next_candidate */
        while (true)
        {
            bool is_prime = true;
            for (size_t i = 0; i < prime_count &&
                  prime_list[i] * prime_list[i] <= next_candidate; ++i)
            {
                if (next_candidate % prime_list[i] == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
            {
                /* Add the prime */
                if (prime_count == prime_capacity)
                {
                    prime_capacity = prime_capacity == 0 ? 16 : prime_capacity * 2;
                    uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                        prime_capacity * sizeof(uint64_t));
                    if (!tmp) abort();           /* allocation failed */
                    prime_list = tmp;
                }
                prime_list[prime_count++] = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
}

/*--- modular arithmetic ---------------------------------------------------*/
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/*--- sequence --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* offset below 2 is irrelevant */

    /* Generate enough primes for this index */
    ensure_primes(n);

    /* p = prime(n), q = prime(n-1) (zero‑based array) */
    uint64_t p = prime_list[(size_t)n - 1];
    uint64_t q = prime_list[(size_t)n - 2];

    /* inverse of q modulo p: q^(p-2) (mod p) */
    uint64_t inv_q = mod_pow_uint64(q, p - 2, p);

    /* k ≡ (p-1) * inv_q (mod p) */
    uint64_t k = ((unsigned __int128)(p - 1) * inv_q) % p;

    /* k is never zero (for prime p), but guard anyway */
    if (k == 0) k = p;
    return (seq_t)k;
}
