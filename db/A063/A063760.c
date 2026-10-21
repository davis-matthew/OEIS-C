/*
 * A063760  Numbers whose sum of non-unitary divisors is a prime and sets a
 *          new record for such primes.
 * https://oeis.org/A063760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 41, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   isPrime128, isPrimeGeneric, isPrimeULL, pow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th square m² such that the difference
 * σ(m²) − usigma(m²) is prime and larger than all previous such primes.
 *
 * The function scans the integers m = 2, 3, …, computes the
 * divisor sums from the factorisation of m, checks the
 * primality of the difference with a deterministic
 * Miller–Rabin test (works for 64‑bit and, if seq_t is
 * unsigned __int128, for 128‑bit values), and keeps
 * the terms that set a new record prime.
 *
 * All integers are stored in seq_t; the code uses only
 * integer arithmetic and works for the standard 8, 16, 32,
 * 64 and 128‑bit integer types as well as an arbitrary‑precision
 * seq_t class in C++.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/isPrime128.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/isPrimeULL.h"
#include "../utils/pow_u64.h"

/* -----------------  Helper:  Miller–Rabin for 128‑bit --------------- */
#if defined(__SIZEOF_INT128__)

#endif

/* -----------------  Prime sieve (up to LIMIT) --------------------- */
#define PRIME_SIEVE_LIMIT 2000000

static int *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list) return;        /* already done */

    /* simple sieve */
    size_t sz = PRIME_SIEVE_LIMIT + 1;
    bool *is_prime = (bool*)malloc(sz);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i*i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i*i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    /* count primes */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;
    prime_list = (int*)malloc(prime_count * sizeof(int));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (int)i;
    free(is_prime);
}

/* -----------------  Factorisation of m --------------------------- */
static size_t factorise(uint64_t m, uint64_t *primes, int *exps)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i]*prime_list[i] <= m; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* -----------------  Cached sequence storage ---------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t max_prime_record = (seq_t)0;

static void grow_terms(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap == 0 ? 64 : terms_cap * 2;
    while (new_cap < need) new_cap *= 2;
    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* -----------------  Main selector -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; 0 is undefined but harmless */

    init_primes();

    /* generate terms until we have n of them */
    uint64_t m = 2;          /* start with 2^2 = 4 */
    while (terms_len < (size_t)n) {
        /* factorise m */
        uint64_t factors[32];
        int exps[32];
        size_t fcnt = factorise(m, factors, exps);

        /* compute sigma(m^2) and usigma(m^2) */
        seq_t sigma = (seq_t)1;
        seq_t usigma = (seq_t)1;
        for (size_t i = 0; i < fcnt; ++i) {
            unsigned full_exp = 2u * (unsigned)exps[i];      /* exponent in m^2 */
            seq_t p_power = pow_u64(factors[i], full_exp);   /* p^(2e)   */
            seq_t p_next  = p_power * (seq_t)factors[i];     /* p^(2e+1) */

            /* sigma factor: (p^(2e+1) - 1)/(p - 1) */
            seq_t numerator = p_next - (seq_t)1;
            seq_t denom     = (seq_t)factors[i] - (seq_t)1;
            seq_t sigma_fac = numerator / denom;

            /* usigma factor: 1 + p^(2e) */
            seq_t usigma_fac = (seq_t)1 + p_power;

            sigma  *= sigma_fac;
            usigma *= usigma_fac;
        }

        seq_t diff = sigma - usigma;   /* sum of non‑unitary divisors */

        /* check if diff is prime and a new record */
        if (diff > max_prime_record && isPrimeGeneric(diff)) {
            /* store square of m as the new term */
            __uint128_t sq128 = (__uint128_t)m * (__uint128_t)m;
            seq_t square = (seq_t)sq128;
            grow_terms(terms_len + 1);
            terms[terms_len++] = square;
            max_prime_record = diff;
        }
        ++m;
    }
    return terms[n - 1];   /* convert to 1‑based index */
}
