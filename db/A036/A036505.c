/*
 * A036505  Numerator of (n+1)^n/n!.
 * https://oeis.org/A036505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036505:  Numerator of (n+1)^n / n!  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static uint64_t *prime_list = NULL;   /* array of primes up to current limit */
static size_t   prime_cnt   = 0;       /* number of primes */
static idx_t    prime_limit = 0;       /* maximum prime pre‑computed */

/* --------------------------------------------------------------------- */
/*  Simple sieve to generate all primes <= limit  */
static void ensure_primes_up_to(idx_t limit) {
    if (limit <= prime_limit) return;           /* already available */

    /* allocate and initialize boolean sieve */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[(size_t)p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[(size_t)m] = false;
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) ++cnt;

    /* allocate new prime array and fill it */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) exit(EXIT_FAILURE);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) new_primes[pos++] = (uint64_t)i;

    /* replace old array */
    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = cnt;
    prime_limit = limit;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/*  Compute a single term a(n)  */
static seq_t compute_term(idx_t n) {
    seq_t result = (seq_t)1;
    idx_t up = n + (idx_t)1;                /* n+1  */

    /* need primes up to up  */
    ensure_primes_up_to(up);

    /* factor n+1  */
    idx_t tmp = up;
    for (size_t idx = 0; idx < prime_cnt && (idx_t)prime_list[idx] * (idx_t)prime_list[idx] <= tmp; ++idx) {
        uint64_t p = prime_list[idx];
        if (tmp % p != 0) continue;

        /* exponent of p in n+1  */
        idx_t e_up = 0;
        while (tmp % p == 0) {
            ++e_up;
            tmp /= p;
        }

        /* exponent of p in (n+1)^n  */
        idx_t e1 = n * e_up;

        /* exponent of p in n! (Legendre's formula)  */
        idx_t e2 = 0;
        idx_t power = p;
        while (power <= n) {
            e2 += n / power;
            if (power > n / p) break;   /* next multiplication would overflow  */
            power *= p;
        }

        idx_t exp = e1 > e2 ? e1 - e2 : 0;
        if (exp > 0) {
            seq_t pow_val = big_pow((seq_t)p, (seq_t)exp);
            result *= pow_val;
        }
    }

    /* if something remains, it is a prime factor > sqrt(up)  */
    if (tmp > 1) {
        uint64_t p = tmp;                 /* p is prime */
        idx_t e_up = 1;                   /* because after division above, tmp is prime */
        idx_t e1 = n * e_up;

        idx_t e2 = 0;
        idx_t power = p;
        while (power <= n) {
            e2 += n / power;
            if (power > n / p) break;
            power *= p;
        }

        idx_t exp = e1 > e2 ? e1 - e2 : 0;
        if (exp > 0) {
            seq_t pow_val = big_pow((seq_t)p, (seq_t)exp);
            result *= pow_val;
        }
    }

    return result;
}

/* --------------------------------------------------------------------- */
/*  Public entry point  */
seq_t a(idx_t n) {
    return compute_term(n);
}
