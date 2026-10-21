/*
 * A076926  Smallest multiple of n with n distinct prime divisors.
 * https://oeis.org/A076926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e||n} p^e) * (∏_{q primes not dividing n, q
 *            smallest (n‑ω(n)) such primes})
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(n) is the number of distinct prime divisors of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static const size_t PRIME_LIMIT = 200000; /* enough for n up to ~10 000 */

/* Ensure the global prime list is built once. */
static void init_primes(void)
{
    if (!primes) {
        build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
    }
}

/* Compute a(n).  n is 1‑based.  Works for any n >= 1. */
seq_t a(idx_t n)
{
    init_primes();

    /* Factorization of n */
    idx_t temp = n;
    seq_t result = 1;
    uint64_t div_primes[64];
    size_t div_cnt = 0;
    size_t i;
    for (i = 0; i < prime_cnt && temp > 1; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) {
            break;
        }
        if (temp % p == 0) {
            div_primes[div_cnt++] = p;   /* record divisor */
            /* multiply by p^exp */
            do {
                temp /= p;
                result *= (seq_t)p;
            } while (temp % p == 0);
        }
    }
    if (temp > 1) {
        /* remainder is a prime > sqrt(original) */
        div_primes[div_cnt++] = temp;
        result *= (seq_t)temp;
    }

    /* Now add enough smallest primes not dividing n to reach n distinct primes */
    size_t need = (size_t)n - div_cnt;
    for (i = 0; need > 0 && i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* skip primes that already divide n */
        bool skip = false;
        for (size_t j = 0; j < div_cnt; ++j) {
            if (div_primes[j] == p) {
                skip = true;
                break;
            }
        }
        if (skip) {
            continue;
        }
        result *= (seq_t)p;
        --need;
    }

    return result;
}
