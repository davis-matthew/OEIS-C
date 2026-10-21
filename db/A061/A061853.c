/*
 * A061853  Difference between smallest prime not dividing n and smallest
 *          nondivisor of n.
 * https://oeis.org/A061853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest_prime_not_dividing(n) - smallest_nondivisor(n)
 * Range:     u8 n <= 9689, u16 n <= 9689, u32 n <= 9689, u64 n <= 9689,
 *            u128 n <= 9689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is undefined; a(1) = 0 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper: generate primes up to 2,000,000 once */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    const uint32_t LIMIT = 2000000U;            /* 2 million */
    bool *is_comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;   /* malloc failed – leave empty */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) {
            if ((uint64_t)i * i <= LIMIT)
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
        }

    /* count primes */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) {
        free(is_comp);
        return;
    }
    size_t k = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_list[k++] = i;

    free(is_comp);
    primes_initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();
    uint64_t nn = (uint64_t)n;

    /* smallest non‑divisor (integer > 1) */
    uint64_t nondiv = 2;
    while (nn % nondiv == 0) ++nondiv;

    /* smallest prime not dividing n */
    uint64_t missing_prime = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (nn % p != 0) {
            missing_prime = p;
            break;
        }
    }

    /* the missing_prime must be found before LIMIT because
       nn cannot contain all primes up to LIMIT when nn fits in 64 bits */
    if (!missing_prime) {
        /* fallback – should never occur for 64‑bit n */
        uint32_t p = prime_list[prime_count - 1] + 1;
        while (true) {
            /* simple primality test: trial division by known primes up to sqrt(p) */
            bool prime = true;
            for (size_t j = 0; j < prime_count && (uint64_t)prime_list[j] * prime_list[j] <= p; ++j)
                if (p % prime_list[j] == 0) { prime = false; break; }
            if (prime && nn % p != 0) {
                missing_prime = p;
                break;
            }
            ++p;
        }
    }

    uint64_t diff = missing_prime - nondiv;
    return (seq_t)diff;
}
