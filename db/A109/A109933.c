/*
 * A109933  Divisors of 10^13 - 1.
 * https://oeis.org/A109933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 10^13‑1 = 9 999 999 999 999 in
 *            increasing order
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gen_divisors_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gen_divisors_recursive.h"

/* ------------ helpers for factorisation and divisor generation ------------ */

#define MAXP 4000000  /* > sqrt(10^13-1) ≈ 3.162e6 */

/* Sieve of Eratosthenes: stores primes in primes[], number of primes in prime_cnt. */
static bool primesieve_done = false;
static uint32_t primes[MAXP / 10]; /* rough upper bound on number of primes */
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (primesieve_done) return;
    bool *is_prime = (bool*)malloc((MAXP + 1) * sizeof(bool));
    for (size_t i = 0; i <= MAXP; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= MAXP; i++) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= MAXP; j += i) is_prime[j] = false;
        }
    }
    for (size_t i = 2; i <= MAXP; i++) {
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;
    }
    free(is_prime);
    primesieve_done = true;
}

static void generate_divisors(uint64_t N, uint64_t **divs, size_t *cnt, size_t *cap) {
    init_primes();
    uint64_t tmp = N;
    uint32_t pf[64];
    uint8_t exp[64];
    size_t pcnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; i++) {
        if (tmp % primes[i] == 0) {
            uint8_t e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                e++;
            }
            pf[pcnt] = primes[i];
            exp[pcnt++] = e;
        }
    }
    if (tmp > 1) {
        pf[pcnt] = (uint32_t)tmp;
        exp[pcnt++] = 1;
    }
    *cap = 1;
    *divs = (uint64_t*)malloc(*cap * sizeof(uint64_t));
    *cnt = 0;
    gen_divisors_recursive(pf, exp, pcnt, 1, 0, *divs, cnt, cap);
}

/* Comparator for qsort */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t u = *(const uint64_t*)a;
    uint64_t v = *(const uint64_t*)b;
    return (u > v) - (u < v);
}

/* ------------------- main sequence function ------------------- */

seq_t a(idx_t n) {
    /* static state for one-time computation of all divisors */
    static bool ready = false;
    static seq_t *divs = NULL;  /* array of divisors (seq_t) */
    static size_t div_cnt = 0;
    if (!ready) {
        /* compute divisors of 10^13 - 1 */
        const uint64_t N = 9999999999999ULL;  /* 10^13 - 1 */
        uint64_t *tmp_divs = NULL;
        size_t tmp_cnt = 0, tmp_cap = 0;
        generate_divisors(N, &tmp_divs, &tmp_cnt, &tmp_cap);
        /* sort */
        qsort(tmp_divs, tmp_cnt, sizeof(uint64_t), cmp_uint64);
        /* convert to seq_t array */
        divs = (seq_t*)malloc(tmp_cnt * sizeof(seq_t));
        for (size_t i = 0; i < tmp_cnt; i++) divs[i] = (seq_t)tmp_divs[i];
        div_cnt = tmp_cnt;
        free(tmp_divs);
        ready = true;
    }
    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= div_cnt) return 0;
    return divs[idx];
}
