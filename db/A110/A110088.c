/*
 * A110088  a(n) = tau(n)^omega(n), where tau = A000005 and omega = A001221.
 * https://oeis.org/A110088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n)^omega(n)
 * Range:     u8 n <= 29, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow_seq.h"
#include "../utils/integer_sqrt.h"

/* Small dynamic prime table for factorization */
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap   = 0;
static uint32_t  max_generated = 1;

/* Generate primes up to 'limit' (inclusive) */
static void ensure_primes(uint32_t limit)
{
    if (limit <= max_generated)
        return;

    for (uint32_t num = max_generated + 1; num <= limit; ++num) {
        bool prime = true;
        uint32_t sqrt_num = (uint32_t)integer_sqrt(num);
        for (size_t i = 0; i < primes_count && primes[i] <= sqrt_num; ++i) {
            if (num % primes[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_count >= primes_cap) {
                size_t new_cap = primes_cap ? (primes_cap << 1) : 16;
                uint32_t *tmp = (uint32_t *)malloc(new_cap * sizeof(uint32_t));
                if (!tmp) exit(1);          /* allocation failed */
                memcpy(tmp, primes, primes_count * sizeof(uint32_t));
                free(primes);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_count++] = num;
        }
    }
    max_generated = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                 /* a(1) = 1 */

    idx_t temp = n;
    seq_t tau = 1;                      /* number of divisors */
    idx_t omega = 0;                    /* number of distinct primes */

    uint32_t limit = (uint32_t)integer_sqrt((idx_t)n);
    ensure_primes(limit);

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if (p > limit || temp == 1)
            break;
        if (temp % p == 0) {
            ++omega;
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
            limit = (uint32_t)integer_sqrt(temp);
        }
    }

    if (temp > 1) {                      /* remaining prime factor */
        ++omega;
        tau *= 2;
    }

    return pow_seq(tau, omega);          /* tau ^ omega */
}
