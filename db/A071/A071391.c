/*
 * A071391  Least number m such that sigma(m) + phi(m) = n or 0 if no such
 *          number exists.
 * https://oeis.org/A071391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal m such that sigma(m)+phi(m)=n, or 0 if no such m
 *            exists
 * Range:     u8 none, u16 n <= 7, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   zalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/zalloc.h"

static const idx_t MAX_M    = 2000000ULL;   /* upper bound for m to sieve */
static const idx_t MAX_SUM  = 6000000ULL;   /* expected maximal sigma+phi for the above m */

static bool     initialized = false;
static seq_t  *m_table       = NULL;      /* m_table[s] = minimal m with sigma(m)+phi(m)=s (0 if unknown) */
static uint32_t *spf_table   = NULL;      /* smallest prime factor array */

/* compute tables once */
static void init_lookup(void)
{
    /* allocate tables */
    spf_table = (uint32_t *)zalloc((size_t)(MAX_M + 1) * sizeof(uint32_t));
    m_table   = (seq_t *)zalloc((size_t)(MAX_SUM + 1) * sizeof(seq_t));

    /* linear sieve for smallest prime factor */
    uint32_t *primes  = (uint32_t *)zalloc((size_t)(MAX_M + 1) * sizeof(uint32_t));
    size_t   prime_cnt = 0;

    for (idx_t i = 2; i <= MAX_M; ++i) {
        if (!spf_table[i]) {
            spf_table[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt && (idx_t)primes[j] * i <= MAX_M; ++j) {
            uint32_t p = primes[j];
            spf_table[p * i] = p;
            if (i % p == 0) break;
        }
    }
    free(primes);

    /* compute sigma(m)+phi(m) for each m and fill the lookup table */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        seq_t sigma = 1;
        seq_t phi   = 1;          /* start with 1, multiply phi(p^e) for each prime factor */

        idx_t tmp = m;
        while (tmp > 1) {
            uint32_t p = spf_table[tmp];
            uint64_t p_pow = 1;
            uint64_t count = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++count;
                p_pow *= (uint64_t)p;
            }
            /* sigma part: (p^(e+1)-1)/(p-1) */
            seq_t sigma_part = (seq_t)((p_pow * (uint64_t)p - 1ULL) / (p - 1ULL));
            sigma *= sigma_part;

            /* phi part: p^(e-1)*(p-1) */
            seq_t phi_part = (seq_t)(p_pow / (uint64_t)p) * (seq_t)(p - 1);
            phi *= phi_part;
        }

        seq_t sum = sigma + phi;
        if ((idx_t)sum <= MAX_SUM) {
            seq_t *slot = m_table + (size_t)sum;
            if (*slot == (seq_t)0) *slot = (seq_t)m;  /* store first (minimal) m */
        }
    }
    free(spf_table);
    initialized = true;
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    if (!initialized) init_lookup();
    if (n <= MAX_SUM) {
        seq_t result = m_table[(size_t)n];
        if (result != (seq_t)0) return result;
    }
    return (seq_t)0;
}
