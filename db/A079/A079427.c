/*
 * A079427  Least m > n having the same number of divisors as n, a(1) = 1.
 * https://oeis.org/A079427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > n with divisor_count(m)=divisor_count(n),
 *            except a(1)=1. The function computes τ(n) by factorisation and
 *            then scans forward until it finds the next integer with the same
 *            divisor count
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- small helper: sieve for primes up to 100000 ---------- */

#define PRIME_LIMIT 100000U

static uint32_t *primes = NULL;          /* array of primes */
static size_t primes_cnt = 0;            /* number of primes stored */
static int primes_inited = 0;            /* flag: list already generated */

static void init_prime_list(void)
{
    if (primes_inited) return;
    primes_inited = 1;

    /* allocate enough space for all primes up to PRIME_LIMIT (≈9592) */
    primes = (uint32_t *)malloc(sizeof(uint32_t) * 10000);
    if (!primes) exit(1);

    /* simple sieve of Eratosthenes */
    char *is_prime = (char *)calloc(PRIME_LIMIT + 1, 1);
    if (!is_prime) exit(1);
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* ---------- divisor count for uint64_t ---------- */

static uint64_t divisor_count_uint64(uint64_t n)
{
    if (n <= 1) return (n == 1) ? 1 : 0;
    uint64_t count = 1;
    init_prime_list();

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (n > 1) {            /* remaining prime factor */
        count *= 2;
    }
    return count;
}

/* ----------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* special case a(1) */
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t tau_n = divisor_count_uint64(nn);

    /* search for the least m > n with the same divisor count */
    for (uint64_t m = nn + 1;; ++m) {
        if (divisor_count_uint64(m) == tau_n)
            return (seq_t)m;
    }
}
