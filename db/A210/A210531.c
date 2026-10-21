/*
 * A210531  Number of nonnegative integers k<n with n+k^3 practical.
 * https://oeis.org/A210531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k < n : n + k³ is a practical number }|
 * Range:     u8 n <= 999, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- Prime sieve up to 1,000,000 ----------------------------------- */
#define PRIME_LIMIT 1000000
static uint32_t primes[80000];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;
    static uint8_t sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) {
            primes[prime_cnt++] = i;
        }
    primes_inited = true;
}

/* --- Practical number test ----------------------------------------- */
static bool is_practical(uint64_t n)
{
    if (n == 1)
        return true;

    uint64_t remaining = n;
    uint64_t sigma = 1;          /* product of sigma values of processed primes */
    bool first_prime = true;     /* true until the first prime factor is handled */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p_sq = (uint64_t)p * (uint64_t)p;
        if (p_sq > remaining)
            break;

        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }

            if (first_prime) {
                if (p != 2)
                    return false;
                first_prime = false;
            } else {
                if ((uint64_t)p > sigma + 1)
                    return false;
            }

            /* sigma factor for p^exp */
            uint64_t pow_val = 1;
            for (int j = 0; j <= exp; ++j)
                pow_val *= p;          /* p^(exp+1) */
            uint64_t sigma_factor = (pow_val - 1) / (p - 1);
            sigma *= sigma_factor;
        }
    }

    if (remaining > 1) {               /* remaining is a prime factor */
        uint64_t p = remaining;
        if (first_prime) {
            if (p != 2)
                return false;
            first_prime = false;
        } else {
            if (p > sigma + 1)
                return false;
        }
        /* exponent is 1: sigma factor = p + 1 */
        sigma *= (p + 1);
    }

    return true;
}

/* --- Sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1, this case not used */

    uint64_t idx = (uint64_t)n;
    uint64_t count = 0;

    init_primes();

    for (uint64_t k = 0; k < idx; ++k) {
        uint64_t cube = k * k * k;   /* k^3 fits in 64 bits (k < 10000) */
        uint64_t m = idx + cube;
        if (is_practical(m))
            ++count;
    }

    return (seq_t)count;
}
