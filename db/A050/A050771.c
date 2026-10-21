/*
 * A050771  Iterated procedure 'composite k added to sum of its prime factors
 *          reaches a prime' yields 4 skipped primes.
 * https://oeis.org/A050771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer k such that iterating k += sum
 *            of its prime factors repeatedly until a prime p is reached, with
 *            exactly 4 primes strictly between k and p
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static bool initialized = false;

/* Sieve state */
static idx_t limit = 2000000;      /* current sieve upper bound */
static bool *is_prime_arr = NULL;  /* boolean sieve array */
static idx_t *prime_list = NULL;   /* primes up to limit */
static size_t prime_cnt = 0;       /* number of primes */

/* Term cache */
static idx_t *terms = NULL;
static size_t terms_size = 0, terms_cap = 0;
static idx_t last_checked = 7;     /* last integer examined */

/* ------------------------------------------------------------------ */
/* Auxiliary: compute sieve of Eratosthenes up to new_limit           */
/* ------------------------------------------------------------------ */
static void compute_sieve(idx_t new_limit)
{
    bool *new_is_prime = (bool *)malloc((size_t)(new_limit + 1));
    for (idx_t i = 0; i <= new_limit; ++i)
        new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_is_prime[j] = false;
        }
    }

    /* build prime list */
    size_t cap = 0;
    idx_t *new_prime_list = NULL;
    size_t cnt = 0;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (new_is_prime[i]) {
            if (cnt == cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                idx_t *tmp = (idx_t *)realloc(new_prime_list,
                                               cap * sizeof(idx_t));
                new_prime_list = tmp;
            }
            new_prime_list[cnt++] = i;
        }
    }

    free(is_prime_arr);
    free(prime_list);
    is_prime_arr = new_is_prime;
    prime_list = new_prime_list;
    prime_cnt = cnt;
    limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Ensure sieve contains at least 'needed'                           */
/* ------------------------------------------------------------------ */
static void ensure_limit(idx_t needed)
{
    if (needed <= limit)
        return;
    idx_t new_limit = limit;
    while (new_limit < needed)
        new_limit = new_limit * 2 + 16;
    compute_sieve(new_limit);
}

/* ------------------------------------------------------------------ */
/* Wrapper: fast primality test using sieve or fallback                  */
/* ------------------------------------------------------------------ */
static inline bool is_prime_fast(idx_t n)
{
    if (n <= limit)
        return is_prime_arr[n];
    return is_prime(n);      /* generic test from utils */
}

/* ------------------------------------------------------------------ */
/* Sum of prime factors (unordered, multiplicity counted)                */
/* ------------------------------------------------------------------ */
static seq_t sum_prime_factors(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > x)
            break;
        while (x % p == 0) {
            sum += p;
            x /= p;
        }
        if (x == 1)
            break;
    }
    if (x > 1)
        sum += x;
    return sum;
}

/* ------------------------------------------------------------------ */
/* Cache of discovered terms                                          */
/* ------------------------------------------------------------------ */
static void add_term(idx_t val)
{
    if (terms_size == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
        idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_size++] = val;
}

/* ------------------------------------------------------------------ */
/* One-time initialization                                           */
/* ------------------------------------------------------------------ */
static void init(void)
{
    compute_sieve(limit);
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public function: a(idx_t n)                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    while (terms_size <= n) {
        /* move to next candidate */
        ++last_checked;
        if (last_checked < 8)
            last_checked = 8;

        if (is_prime_fast(last_checked))
            continue;           /* skip primes */

        idx_t k = last_checked;
        idx_t cur = k;

        /* iterate until a prime is reached */
        while (true) {
            if (is_prime_fast(cur))
                break;
            seq_t sum = sum_prime_factors(cur);
            cur = (idx_t)(cur + sum);
            if (cur > limit)
                ensure_limit(cur);
        }

        /* cur is the reached prime */
        idx_t prime_final = cur;

        /* count primes strictly between k and prime_final */
        uint32_t primes_between = 0;
        for (idx_t x = k + 1; x < prime_final; ++x) {
            if (is_prime_fast(x)) {
                ++primes_between;
                if (primes_between > 4)
                    break;
            }
        }

        if (primes_between == 4)
            add_term(k);
    }

    return (seq_t)terms[n];
}
