/*
 * A066692  Odd numbers k such that tau(k) = tau(k+2), where tau(k) =
 *          A000005(k) is the number of divisors of k.
 * https://oeis.org/A066692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd k such that τ(k) = τ(k+2) where τ(n) = number of
 *            divisors of n
 * Range:     u8 n <= 37, u16 n <= 6811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: integer square root (floor) for uint64_t */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Prime sieve up to a dynamic limit.  Stores all primes in a static array. */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static uint32_t primes_limit = 0;

static void init_primes(uint32_t limit)
{
    if (limit <= primes_limit)
        return;

    /* Allocate boolean array for sieve */
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime)
        exit(1);

    for (uint32_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_primes)
        exit(1);
    primes = new_primes;
    primes_cnt = cnt;
    primes_limit = limit;

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = i;

    free(is_prime);
}

/* Compute τ(n) for uint64_t n using the precomputed primes. */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1)
        return 1; /* τ(1)=1, τ(0) is not used here */

    uint64_t result = 1;
    uint64_t m = n;
    uint64_t sqrt_m = isqrt_u64(m);
    if (sqrt_m > primes_limit)
        init_primes((uint32_t)sqrt_m + 1);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (m > 1) { /* Remaining prime factor */
        result *= 2;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1 – undefined for 0 */
        return (seq_t)0;

    /* Static storage for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0, terms_cap = 0;
    static uint64_t last_checked = 1; /* last odd number processed */

    while (terms_cnt < (size_t)n) {
        uint64_t k = last_checked;

        uint64_t tau_k = divisor_count_u64(k);
        uint64_t tau_k2 = divisor_count_u64(k + 2);

        if (tau_k == tau_k2) {
            if (terms_cnt == terms_cap) { /* grow array */
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms)
                    exit(1);
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)k;
        }

        last_checked += 2; /* next odd */
    }

    return terms[n - 1];
}
