/*
 * A104480  Numbers m such that the period length P(m) of the Fibonacci
 *          sequence modulo m is a perfect square.
 * https://oeis.org/A104480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   fib_pair_fast, gen_divisors, is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104480:
 * Numbers m such that the Pisano period P(m) of the Fibonacci sequence
 * modulo m is a perfect square.
 * Computation: iterate m = 1, 2, …, compute P(m) via prime‑factorisation
 * using known period formulas, test square, and return the nth term. */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/is_square.h"
#include "../utils/fib_pair_fast.h"
#include "../utils/gen_divisors.h"

/* fast gcd for 64‑bit values */
static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* prime list generated once per call */
static uint64_t *global_primes = NULL;
static size_t global_primes_cnt = 0;

/* factorises n using global_primes; result stored in primes[], cnts[] */
static void factorize(uint64_t n, uint64_t primes[], uint64_t cnts[], size_t *pf_cnt) {
    *pf_cnt = 0;
    for (size_t i = 0; i < global_primes_cnt && global_primes[i] * global_primes[i] <= n; ++i) {
        uint64_t p = global_primes[i];
        if (n % p == 0) {
            primes[*pf_cnt] = p;
            cnts[*pf_cnt]   = 0;
            while (n % p == 0) { n /= p; ++cnts[*pf_cnt]; }
            ++(*pf_cnt);
        }
    }
    if (n > 1) {
        primes[*pf_cnt] = n;
        cnts[*pf_cnt]   = 1;
        ++(*pf_cnt);
    }
}

/* Computes P(p) for a prime p (p > 5) */
static uint64_t compute_prime_period(uint64_t p) {
    if (p == 5) return 20;
    uint64_t candidate = (p % 10 == 1 || p % 10 == 9) ? p - 1 : 2 * (p + 1);

    uint64_t pf[32], cnt[32];
    size_t pf_cnt;
    factorize(candidate, pf, cnt, &pf_cnt);

    uint64_t divs[2000];
    size_t div_cnt = 0;
    gen_divisors(0, 1, pf, cnt, pf_cnt, divs, &div_cnt);

    /* divisors are not guaranteed sorted, so scan all */
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        uint64_t f, f1;
        fib_pair_fast(d, p, &f, &f1);
        if (f == 0 && f1 == 1) return d;
    }
    return candidate;  /* safety, should never happen */
}

/* Pisano period for arbitrary modulus m (m >= 1) */
static uint64_t pisano_period_mod(uint64_t m, uint64_t *spf, uint64_t *prime_period_cache) {
    uint64_t res = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        uint64_t exp = 0;
        while (m % p == 0) { m /= p; ++exp; }

        uint64_t per;
        if (p == 2) {
            if (exp == 1) per = 3;
            else if (exp == 2) per = 6;
            else per = 3ULL << (exp - 1); /* 3*2^{e-1} */
        } else if (p == 5) {
            per = 20;
            for (uint64_t i = 1; i < exp; ++i) per *= 5; /* 20*5^{e-1} */
        } else {
            if (prime_period_cache[p] == 0)
                prime_period_cache[p] = compute_prime_period(p);
            per = prime_period_cache[p];
            for (uint64_t i = 1; i < exp; ++i) per *= p;
        }

        uint64_t g = gcd64(res, per);
        res = (res / g) * per;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Estimate upper bound for m to include n terms – heuristic */
    uint64_t limit = (uint64_t)n * 200ULL + 1000ULL;
    if (limit < 5000ULL) limit = 5000ULL;

    /* Build smallest‑prime‑factor array up to limit */
    uint64_t *spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Build list of primes up to sqrt(2*limit)+2 for factorisation */
    uint64_t sieve_lim = (uint64_t)(sqrt((double)(2ULL * limit + 2ULL))) + 2ULL;
    bool *is_composite = (bool *)calloc(sieve_lim + 1, sizeof(bool));
    global_primes = (uint64_t *)malloc((sieve_lim + 1) * sizeof(uint64_t));
    global_primes_cnt = 0;
    for (uint64_t i = 2; i <= sieve_lim; ++i) {
        if (!is_composite[i]) {
            global_primes[global_primes_cnt++] = i;
            if (i * i <= sieve_lim) {
                for (uint64_t j = i * i; j <= sieve_lim; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);

    /* Cache for prime periods */
    uint64_t *prime_period_cache = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));

    /* Iterate over m, collecting terms */
    seq_t result = 0;
    uint64_t found = 0;
    for (uint64_t m = 1; m <= limit && found < n; ++m) {
        uint64_t per = pisano_period_mod(m, spf, prime_period_cache);
        if (is_square((seq_t)per)) {
            ++found;
            if (found == (uint64_t)n) result = (seq_t)m;
        }
    }

    /* Clean up */
    free(spf);
    free(prime_period_cache);
    free(global_primes);

    return result;
}
