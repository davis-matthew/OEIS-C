/*
 * A212127  Numbers n whose arithmetic derivative equals the sum of its proper
 *          divisors.
 * https://oeis.org/A212127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that
 *            arithmetic_derivative(m) equals sum_of_proper_divisors(m)
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_uint64_array, free_uint64_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_uint64_array.h"
#include "../utils/free_uint64_array.h"

/* -------------------------------------------------------------------------- */
/*  Helpers for conditional allocation between C and C++                         */

static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* -------------------------------------------------------------------------- */
/*  Global state                                                               */
/* -------------------------------------------------------------------------- */
static uint64_t *primes = NULL;     /* primes found so far          */
static size_t primes_cnt = 0;       /* number of primes stored      */
static size_t primes_cap = 0;       /* capacity of primes array     */
static uint64_t primes_limit = 0;   /* largest number for which primes are known */

static seq_t *terms = NULL;          /* sequence terms found so far    */
static size_t terms_cnt = 0;         /* number of terms stored         */
static size_t terms_cap = 0;         /* capacity of terms array       */
static uint64_t last_checked = 0;    /* largest integer examined       */

/* -------------------------------------------------------------------------- */
/*  Ensure the prime list contains all primes ≤ limit.                         */
/* -------------------------------------------------------------------------- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= primes_limit) return;

    if (primes_limit < 2) primes_limit = 1;

    for (uint64_t cand = primes_limit + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint64_t *tmp = alloc_uint64_array(new_cap);
                for (size_t i = 0; i < primes_cnt; ++i) tmp[i] = primes[i];
                free_uint64_array(primes);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = cand;
        }
    }
    primes_limit = limit;
}

/* -------------------------------------------------------------------------- */
/*  Ensure the terms array has enough capacity.                               */
/* -------------------------------------------------------------------------- */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = alloc_seq_array(new_cap);
    for (size_t i = 0; i < terms_cnt; ++i) tmp[i] = terms[i];
    free_seq_array(terms);
    terms = tmp;
    terms_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/*  Arithmetic derivative of a uint64_t number.                                */
/* -------------------------------------------------------------------------- */
static seq_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t der = (seq_t)0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            der += (seq_t)exp * (seq_t)(n / p);
        }
    }
    if (tmp > 1) {
        der += (seq_t)1 * (seq_t)(n / tmp);
    }
    return der;
}

/* -------------------------------------------------------------------------- */
/*  Sum of proper divisors (σ(n) – n) for a uint64_t number.                    */
/* -------------------------------------------------------------------------- */
static seq_t sum_proper_divisors(uint64_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sigma = (seq_t)1;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            seq_t term = (seq_t)1;
            seq_t power = (seq_t)1;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
        }
    }
    if (tmp > 1) {
        sigma *= ((seq_t)1 + (seq_t)tmp);
    }
    return sigma - (seq_t)n;
}

/* -------------------------------------------------------------------------- */
/*  Generate sequence terms until the desired count is reached.              */
/* -------------------------------------------------------------------------- */
static void generate_up_to(size_t desired_count)
{
    if (terms_cnt >= desired_count) return;

    while (terms_cnt < desired_count) {
        uint64_t candidate = last_checked + 1;
        ensure_primes_upto(candidate);

        seq_t der = arithmetic_derivative(candidate);
        seq_t sum = sum_proper_divisors(candidate);
        if (der == sum) {
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)candidate;
        }
        last_checked = candidate;
    }
}

/* -------------------------------------------------------------------------- */
/*  Public function required by the test harness.                              */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for n = 0 */
    generate_up_to((size_t)n);
    return terms[n - 1];                      /* 0‑based array index */
}
