/*
 * A102187  Arithmetic means of divisors of arithmetic numbers (arithmetic
 *          numbers, A003601, are those for which the average of the divisors
 *          is an integer).
 * https://oeis.org/A102187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A003601(n))/tau(A003601(n)), where a(n) is the
 *            average of the divisors of the nth arithmetic number (numbers
 *            with integer average of divisors)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A102187. */
#include <stdlib.h>
#include <stdint.h>

/* Helper: power of an unsigned integer base to a small exponent,
   all in seq_t domain. */
static inline seq_t pow_seq(seq_t base, unsigned exp) {
    seq_t result = 1;
    while (exp--) result *= base;
    return result;
}

seq_t a(idx_t n) {
    /* Pre‑computation parameters */
    static const idx_t LIMIT = 400000;          /* upper bound for our sieve */
    static uint32_t *spf = NULL;               /* smallest prime factor array */
    static seq_t *a_vals = NULL;               /* averages of arithmetic numbers */
    static size_t a_cnt = 0;                   /* how many have been stored */
    static size_t a_cap = 0;                   /* capacity of a_vals array */
    static int init = 0;                       /* init flag */

    /* Lazy initialization */
    if (!init) {
        /* Allocate and build smallest prime factor array */
        spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * (uint64_t)i <= LIMIT) {
                    for (idx_t j = i * i; j <= LIMIT; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }

        /* Allocate array for averages */
        a_cap = 1024;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));

        /* --------------------------------------------------------------------
           Compute sigma and tau for every integer up to LIMIT.
           When sigma(n) % tau(n) == 0, n is arithmetic; store average.
           -------------------------------------------------------------------- */
        /* n = 1 is arithmetic */
        a_vals[a_cnt++] = 1;

        for (idx_t i = 2; i <= LIMIT; ++i) {
            seq_t sigma_val = 1;
            seq_t tau_val   = 1;
            idx_t x = i;
            while (x > 1) {
                uint32_t p = spf[x];
                unsigned e = 0;
                while (x % p == 0) {
                    x /= p;
                    ++e;
                }
                /* sigma factor: (p^{e+1}-1)/(p-1) */
                seq_t pe1 = pow_seq((seq_t)p, e + 1);
                seq_t sig_factor = (pe1 - 1) / ((seq_t)p - 1);
                sigma_val *= sig_factor;
                tau_val   *= (seq_t)(e + 1);
            }
            if (sigma_val % tau_val == 0) {
                seq_t avg = sigma_val / tau_val;
                if (a_cnt == a_cap) {
                    a_cap *= 2;
                    a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
                }
                a_vals[a_cnt++] = avg;
            }
        }

        init = 1;
    }

    /* Ensure we have enough terms calculated */
    if ((idx_t)(n - 1) >= a_cnt) {
        /* In practice this should never happen with LIMIT=400000
           and n <= 10000. Return 0 as a safe fallback. */
        return 0;
    }
    return a_vals[n - 1];
}
