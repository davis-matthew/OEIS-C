/*
 * A029935  a(n) = Sum_{d divides n} phi(d)*phi(n/d).
 * https://oeis.org/A029935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)*phi(n/d) = (phi * phi)(n)
 * Range:     u8 n <= 84, u16 n <= 7244, u32 n <= 8205, u64 n <= 8205,
 *            u128 n <= 8205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* cached primes up to 1,000,000 (covers sqrt(10^12)) */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    const uint64_t limit = 1000000;
    bool *mark = (bool*)calloc(limit + 1, 1);
    /* allocate enough space for primes up to limit */
    primes = (uint64_t*)malloc(80000 * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            primes[idx++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    mark[j] = true;
            }
        }
    }
    prime_cnt = idx;
    free(mark);
    primes_inited = true;
}

/* factorize n into primes and exponents (arrays must be large enough) */
static void factor_uint64(uint64_t n, uint64_t *pbuf, int *ebuf, size_t *cnt) {
    size_t i = 0;
    for (size_t k = 0; k < prime_cnt && primes[k] * primes[k] <= n; ++k) {
        if (n % primes[k] == 0) {
            int e = 0;
            while (n % primes[k] == 0) {
                n /= primes[k];
                ++e;
            }
            pbuf[i] = primes[k];
            ebuf[i] = e;
            ++i;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        pbuf[i]   = n;
        ebuf[i]   = 1;
        ++i;
    }
    *cnt = i;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* offset is 1, but guard anyway */

    if (!primes_inited) init_primes();

    seq_t result = 1;
    uint64_t nn = (uint64_t)n;

    /* factorization buffers; for 64‑bit n 64 factors are enough */
    uint64_t p[64];
    int     e[64];
    size_t  factor_cnt;
    factor_uint64(nn, p, e, &factor_cnt);

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t prime = p[i];
        int exp = e[i];

        /* compute p^exp */
        seq_t pow_p = 1;
        for (int k = 0; k < exp; ++k)
            pow_p *= (seq_t)prime;

        seq_t pow_p_prev = pow_p / (seq_t)prime;           /* p^(exp-1) */
        seq_t term1 = (seq_t)(exp + 1) * (pow_p - pow_p_prev);
        seq_t term2 = 0;

        if (exp >= 2) {
            seq_t pow_p_prev2 = pow_p_prev / (seq_t)prime;   /* p^(exp-2) */
            seq_t diff2 = pow_p_prev - pow_p_prev2;
            term2 = (seq_t)(exp - 1) * diff2;
        }

        seq_t a_pe = term1 - term2;
        result *= a_pe;
    }
    return result;
}
