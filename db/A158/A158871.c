/*
 * A158871  Those positive integers m where (the number of integers k, 0 <= k
 *          <= m, where the binomial coefficient C(m,k) is divisible by m) >
 *          (the number of positive integers <= m and coprime to m).
 * https://oeis.org/A158871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes A158871. For each integer m, A020475(m) =
 *            #{k | 0≤k≤m, C(m,k) divisible by m} A000010(m) = φ(m) m is in
 *            the sequence iff A020475(m) > φ(m). The function generates the
 *            sequence by a sieve and pre‑computed prime‑factor valuations up
 *            to a fixed limit
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define LIM 20000U          /* upper bound for generated m */

seq_t a(idx_t n)
{
    /* static data required for all calls */
    static bool init = false;
    static idx_t *spf = NULL;           /* smallest prime factor */
    static int *prime_id = NULL;        /* prime → index in primes[] */
    static idx_t *primes = NULL;        /* list of primes */
    static size_t prime_cnt = 0;
    static uint16_t **vp_fact = NULL;    /* vp_fact[prime_index][t] = v_p(t!) */
    static seq_t seq_terms[LIM+1];       /* cached sequence terms */
    static size_t seq_len = 0;
    static idx_t next_m = 1;

    if (!init) {
        /* ---------- sieve and arrays ----------
           Computes spf, prime_id, primes, and vp_fact. ---------- */
        spf = (idx_t*)malloc((LIM + 1) * sizeof(idx_t));
        prime_id = (int*)malloc((LIM + 1) * sizeof(int));
        primes = (idx_t*)malloc((LIM + 1) * sizeof(idx_t));

        for (size_t i = 0; i <= LIM; ++i) spf[i] = 0;
        for (size_t i = 0; i <= LIM; ++i) prime_id[i] = -1;

        for (idx_t i = 2; i <= LIM; ++i) {
            if (!spf[i]) {
                spf[i] = i;
                prime_id[i] = (int)prime_cnt;
                primes[prime_cnt++] = i;
                if ((i * i) <= LIM) {
                    for (idx_t j = i * i; j <= LIM; j += i)
                        if (!spf[j]) spf[j] = i;
                }
            }
        }

        vp_fact = (uint16_t**)malloc(prime_cnt * sizeof(uint16_t*));
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = primes[pi];
            uint16_t* arr = (uint16_t*)malloc((LIM + 1) * sizeof(uint16_t));
            vp_fact[pi] = arr;
            uint16_t val = 0;
            arr[0] = 0;
            for (idx_t t = 1; t <= LIM; ++t) {
                idx_t tmp = t;
                uint16_t cnt = 0;
                while ((tmp % p) == 0) {
                    tmp /= p;
                    ++cnt;
                }
                val += cnt;
                arr[t] = val;
            }
        }
        init = true;
    }

    /* generate terms until we have enough */
    while (seq_len < n) {
        idx_t m = next_m;
        /* factorization of m */
        idx_t temp = m;
        int idx_e[10];      /* prime indices in primes[] */
        int exp_e[10];      /* corresponding exponents */
        size_t fac_cnt = 0;
        while (temp > 1) {
            idx_t p = spf[temp];
            int pi = prime_id[p];
            int e = 0;
            while ((temp % p) == 0) {
                temp /= p;
                ++e;
            }
            idx_e[fac_cnt] = pi;
            exp_e[fac_cnt] = e;
            ++fac_cnt;
        }

        /* compute φ(m) */
        idx_t phi_val = m;
        for (size_t i = 0; i < fac_cnt; ++i) {
            idx_t p = primes[idx_e[i]];
            phi_val -= phi_val / p;
        }

        /* compute A020475(m) by checking all k */
        size_t div_count = 0;
        for (idx_t k = 0; k <= m; ++k) {
            bool ok = true;
            for (size_t i = 0; i < fac_cnt; ++i) {
                int pi = idx_e[i];
                int e_req = exp_e[i];
                uint16_t total = vp_fact[pi][m];
                uint16_t vpk   = vp_fact[pi][k];
                uint16_t vpmk  = vp_fact[pi][m - k];
                if ((total - vpk - vpmk) < (uint16_t)e_req) {
                    ok = false;
                    break;
                }
            }
            if (ok) ++div_count;
        }

        if (div_count > phi_val) {
            seq_terms[seq_len++] = (seq_t)m;
        }
        ++next_m;
    }

    return seq_terms[n - 1];
}
