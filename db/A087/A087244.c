/*
 * A087244  Nonsquarefree deficient numbers.
 * https://oeis.org/A087244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 5037, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A087244 – Nonsquare‑free deficient numbers
   Computation: iterate integers m ≥ 2, factor each m,
   test that m is not square‑free and that σ(m) < 2·m.
   The first n such m's are cached in a static array. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* integer square root for 64‑bit values                              */
/* ------------------------------------------------------------------ */
static uint32_t floor_sqrt_uint64(uint64_t v)
{
    uint64_t lo = 0, hi = (uint64_t)0xFFFFFFFFULL;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == v) return (uint32_t)mid;
        if (sq < v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (uint32_t)ans;
}

/* ------------------------------------------------------------------ */
/* prime list handling                                                 */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list   = NULL;   /* dynamic array of primes   */
static size_t    prime_cnt    = 0;      /* number of primes stored   */
static uint32_t  next_test    = 2;      /* next integer to test     */

/* append a prime to the list, resizing if needed                     */
static void append_prime(uint32_t p)
{
    if (prime_cnt == 0) {
        prime_list = (uint32_t *)malloc(64 * sizeof(uint32_t));
    } else if (prime_cnt % 64 == 0) {
        uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                            (prime_cnt + 64) * sizeof(uint32_t));
        if (tmp) prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* test whether x is prime using the current prime list               */
static bool is_prime(uint32_t x)
{
    if (x < 2) return false;
    uint32_t lim = floor_sqrt_uint64(x);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p > lim) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* make sure all primes ≤ limit are present in prime_list             */
static void ensure_primes_up_to(uint32_t limit)
{
    if (limit < next_test) return;
    for (uint32_t x = next_test; x <= limit; ++x) {
        if (is_prime(x)) append_prime(x);
    }
    next_test = limit + 1;
}

/* ------------------------------------------------------------------ */
/* test whether m is a nonsquarefree deficient number                 */
/* ------------------------------------------------------------------ */
static bool is_nonsqfree_deficient(uint64_t m)
{
    uint64_t temp = m;
    seq_t sigma = 1;
    bool not_sqfree = false;

    uint32_t sqrt_m = floor_sqrt_uint64(m);
    ensure_primes_up_to(sqrt_m);

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p != 0) continue;
        int exp = 0;
        seq_t term = 1; /* will become 1 + p + p^2 + ... + p^e */
        while (temp % p == 0) {
            temp /= p;
            ++exp;
            term = term * (seq_t)p + (seq_t)1;
        }
        if (exp > 1) not_sqfree = true;
        sigma *= term;
    }

    if (temp > 1) { /* remaining prime factor with exponent 1 */
        seq_t term = (seq_t)temp + (seq_t)1; /* p^0 + p^1 = 1 + p */
        sigma *= term;
        /* exponent is 1, so not_sqfree unchanged */
    }

    if (!not_sqfree) return false;
    seq_t twice_m = (seq_t)2 * (seq_t)m;
    return sigma < twice_m;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of found terms                                */
    static seq_t *terms = NULL;
    static size_t  terms_cap = 0;
    static size_t  terms_cnt = 0;

    /* last integer checked                                       */
    static uint64_t last_checked = 2;

    /* grow storage for terms when needed                          */
    if (terms_cap < (size_t)n) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* generate terms until we have at least n of them             */
    while (terms_cnt < (size_t)n) {
        if (is_nonsqfree_deficient(last_checked)) {
            terms[terms_cnt++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
    return terms[n - 1];
}
