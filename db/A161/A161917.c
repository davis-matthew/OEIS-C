/*
 * A161917  Numbers for which the sum of their prime factors (with repetition)
 *          divides the sum of their divisors.
 * https://oeis.org/A161917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 325, u32 n <= 4473, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161917: k such that the sum of their prime factors divides the sum
   of their divisors. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Integer sqrt (floor) for uint64_t. */
static inline uint64_t int_sqrt_u64(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Prime table (generated on demand). */
static uint64_t *prime_list      = NULL;
static size_t prime_cap          = 0;
static size_t prime_cnt          = 0;
static uint64_t prime_limit      = 1;   /* largest number tested for prime */

/* Expand the prime list up to 'limit' (inclusive). */
static void build_primes_upto(uint64_t limit) {
    if (limit <= prime_limit) return;
    for (uint64_t n = prime_limit + 1; n <= limit; ++n) {
        if (n < 2) continue;
        bool isp = true;
        uint64_t r = int_sqrt_u64(n);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (n % prime_list[i] == 0) { isp = false; break; }
        }
        if (isp) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = n;
        }
    }
    prime_limit = limit;
}

/* Test whether k satisfies the property. */
static inline bool satisfies(uint64_t k) {
    seq_t sumPF = 0;   /* sum of prime factors (with multiplicity) */
    seq_t sigma = 1;   /* sum of divisors */
    uint64_t n = k;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n / prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            sumPF += (seq_t)p * e;

            /* term = (p^(e+1)-1)/(p-1) */
            seq_t ppow = 1;
            for (int j = 0; j < e + 1; ++j) ppow *= (seq_t)p;
            seq_t term = (ppow - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        uint64_t p = n;
        sumPF += (seq_t)p;             /* exponent is 1 */
        seq_t term = ((seq_t)p * (seq_t)p - 1) / ((seq_t)p - 1);  /* equals p+1 */
        sigma *= term;
    }
    return sumPF && (sigma % sumPF == 0);
}

/* ----------------------------------------------------- */
/*  seq_t a(idx_t n) – sequence implementation.        */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* persistent state */
    static seq_t *terms     = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t cur_k   = 2;   /* next candidate to test */

    while (terms_len < n) {
        uint64_t limit = int_sqrt_u64(cur_k);
        build_primes_upto(limit);

        if (satisfies(cur_k)) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)cur_k;
        }
        ++cur_k;
    }
    return terms[n - 1];
}
