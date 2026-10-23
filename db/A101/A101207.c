/*
 * A101207  For each prime power n, a(n) is the number of positive integers
 *          that have n as their greatest prime power.
 * https://oeis.org/A101207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 unless n = p^k for some prime p and k ≥ 1. For n = p^k,
 *            a(p^k) = (∏_{q prime, q ≤ p^k} (⌊log_q(p^k)⌋ + 1)) / (k + 1).
 *            The product runs over all primes q ≤ n; the factor for q = p
 *            equals k+1 and is cancelled by the division. This implementation
 *            computes the product directly using integer...
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAXSIEVE 200000

static bool sieve_done = false;
static bool is_prime[MAXSIEVE + 1];
static unsigned int prime_list[MAXSIEVE / 10];
static unsigned int prime_cnt = 0;

static void init_sieve(void)
{
    for (unsigned int i = 0; i <= MAXSIEVE; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (unsigned int i = 2; (unsigned long long)i * i <= MAXSIEVE; ++i)
        if (is_prime[i])
            for (unsigned int j = i * i; j <= MAXSIEVE; j += i)
                is_prime[j] = false;
    for (unsigned int i = 2; i <= MAXSIEVE; ++i)
        if (is_prime[i])
            prime_list[prime_cnt++] = i;
    sieve_done = true;
}

seq_t a(idx_t n)
{
    if (!sieve_done)
        init_sieve();

    if (n == 1u)
        return (seq_t)1;

    /* Find if n is a prime power and record its base and exponent. */
    idx_t temp = n;
    idx_t base = 0;   /* prime base */
    idx_t exp = 0;    /* exponent k */

    /* Check factors using the precomputed prime list. */
    for (unsigned int i = 0; i < prime_cnt && (unsigned long long)prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            base = p;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            break;
        }
    }

    /* If no factor found, n is prime. */
    if (base == 0) {
        base = n;
        exp = 1;
        temp = 1;
    }

    /* If any prime factor remains, n is not a prime power. */
    if (temp != 1)
        return (seq_t)0;

    /* Compute the product over all primes q ≤ n. */
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < prime_cnt && prime_list[i] <= n; ++i) {
        idx_t q = prime_list[i];
        /* Compute floor(log_q(n)) by repeated multiplication. */
        idx_t power = 1;
        unsigned int e = 0;
        while (power <= n / q) {
            power *= q;
            ++e;
        }
        result *= (seq_t)(e + 1);
    }

    /* Divide by (exp + 1) to cancel the factor for the prime base. */
    result /= (seq_t)(exp + 1);

    return result;
}
