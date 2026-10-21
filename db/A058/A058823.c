/*
 * A058823  a(0) = 1, a(1) = 8; for n >= 2 a(n) is the number of degree-n
 *          monic reducible polynomials over GF(8), i.e., a(n) = 8^n -
 *          A027380(n).
 * https://oeis.org/A058823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 8; for n >= 2 : a(n) = 8^n - (1/n) * Σ_{d|n}
 *            μ(d) * 8^{n/d} where μ is the Möbius function. All arithmetic is
 *            performed in seq_t. The function memoizes results and grows its
 *            internal tables as needed
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static idx_t max_computed = 0;          /* largest n we have computed already                     */
    static seq_t *vals = NULL;             /* a(0) .. a(max_computed)                                */
    static int *mu = NULL;                 /* Möbius values, size max_computed+1                      */
    static idx_t *spf = NULL;              /* smallest prime factor table, size max_computed+1        */
    static idx_t *primes = NULL;           /* list of primes found                                 */
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *pow8 = NULL;             /* 8^i for i = 0 .. max_computed                         */
    static size_t pow_len = 0;

    /* ---------- special cases ---------- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* ---------- ensure tables are big enough ---------- */
    if (n > max_computed)
    {
        /* ---- grow the array of powers of 8 ---- */
        if (pow_len == 0) {
            pow_len = 1;
            pow8 = (seq_t *)realloc(NULL, pow_len * sizeof(seq_t));
            pow8[0] = (seq_t)1;
        }
        if (n + 1 > pow_len) {
            size_t new_len = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(pow8, new_len * sizeof(seq_t));
            if (!tmp) abort();
            pow8 = tmp;
            for (size_t i = pow_len; i <= (size_t)n; ++i)
                pow8[i] = pow8[i-1] * (seq_t)8U;
            pow_len = new_len;
        }

        /* ---- grow the Möbius, spf, and prime tables ---- */
        if (max_computed < n) {
            size_t new_max = (size_t)n + 1;

            /* mu array */
            int *tmp_mu = (int *)realloc(mu, new_max * sizeof(int));
            if (!tmp_mu) abort();
            mu = tmp_mu;

            /* spf array */
            idx_t *tmp_spf = (idx_t *)realloc(spf, new_max * sizeof(idx_t));
            if (!tmp_spf) abort();
            spf = tmp_spf;

            /* initialize arrays */
            mu[0] = 0;
            mu[1] = 1;
            spf[0] = 0;
            spf[1] = 1;
            for (size_t i = 2; i < new_max; ++i) spf[i] = 0;

            /* linear sieve for primes and spf, then compute μ */
            for (size_t i = 2; i < new_max; ++i) {
                if (spf[i] == 0) {                /* i is prime */
                    spf[i] = (idx_t)i;
                    if (primes_cnt == primes_cap) {
                        primes_cap = primes_cap ? primes_cap * 2 : 16;
                        idx_t *tmp_pr = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                        if (!tmp_pr) abort();
                        primes = tmp_pr;
                    }
                    primes[primes_cnt++] = (idx_t)i;
                }
                for (size_t j = 0; j < primes_cnt; ++j) {
                    idx_t p = primes[j];
                    size_t v = i * (size_t)p;
                    if (v >= new_max) break;
                    spf[v] = p;
                    if (i % p == 0) break;
                }
            }

            /* compute Möbius */
            mu[0] = 0;
            mu[1] = 1;
            for (size_t i = 2; i < new_max; ++i) {
                size_t p = spf[i];
                size_t m = i / p;
                if (m % p == 0)
                    mu[i] = 0;
                else
                    mu[i] = -mu[m];
            }

            /* extend value array */
            seq_t *tmp_vals = (seq_t *)realloc(vals, new_max * sizeof(seq_t));
            if (!tmp_vals) abort();
            vals = tmp_vals;

            /* initialise a(0) and a(1) if not already */
            if (max_computed < 1) {
                vals[0] = (seq_t)1;
                vals[1] = (seq_t)8;
                max_computed = 1;
            }
        }

        /* ---- compute values for new indices ---- */
        for (size_t i = (size_t)(max_computed + 1); i <= (size_t)n; ++i) {
            /* compute irreducible count I(i) = (1/i) * Σ_{d|i} μ(d) * 8^{i/d} */
            seq_t sum = (seq_t)0;
            for (size_t d = 1; d * d <= i; ++d) {
                if (i % d != 0) continue;

                int md = mu[d];
                if (md != 0) {
                    seq_t term = pow8[i / d];
                    if (md > 0)
                        sum += term;
                    else
                        sum -= term;
                }

                size_t d2 = i / d;
                if (d2 != d) {
                    int md2 = mu[d2];
                    if (md2 != 0) {
                        seq_t term = pow8[d];          /* 8^{i/d2} == 8^{d} */
                        if (md2 > 0)
                            sum += term;
                        else
                            sum -= term;
                    }
                }
            }
            seq_t irreducible = sum / (seq_t)i;          /* exact division */
            seq_t reducible = pow8[i] - irreducible;
            vals[i] = reducible;
        }
        max_computed = (size_t)n;
    }

    return vals[n];
}
