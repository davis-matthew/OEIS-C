/*
 * A077614  Let F(m,k) be the number of positive integers j <= m such that
 *          gcd(j,k) = 1; then a(n) is the number of positive integers k such
 *          that F(n,k) = k.
 * https://oeis.org/A077614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of k <= n such that F(n,k) = #{ j <= n |
 *            gcd(j,k)=1 } equals k
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t max_precomputed = 0;
static idx_t *global_spf = NULL;
static seq_t *global_phi  = NULL;

/* Pre‑compute smallest prime factor and Euler’s totient up to n. */
static void precompute(idx_t n) {
    if (n <= max_precomputed) return;
    if (global_spf) free(global_spf);
    if (global_phi) free(global_phi);
    build_spf(n, &global_spf);
    global_phi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!global_phi) abort();
    global_phi[0] = 0;
    global_phi[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p  = global_spf[i];
        idx_t m  = i / p;
        if ((m % p) == 0) {
            global_phi[i] = global_phi[m] * p;
        } else {
            global_phi[i] = global_phi[m] * (p - 1);
        }
    }
    max_precomputed = n;
}

/* Return the number of positive integers k such that
   F(n,k) = k, where F(n,k) is the count of integers <= n
   coprime to k. */
seq_t a(idx_t n) {
    precompute(n);
    seq_t ans = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;         /* how many full blocks of size k */
        idx_t r = n % k;         /* remainder part */
        seq_t phi_k = global_phi[k];

        /* Compute R = # { 1 <= j <= r | gcd(j,k)=1 } */
        seq_t R;
        if (r == 0) {
            R = 0;
        } else {
            /* distinct prime factors of k */
            uint32_t primes[16];
            uint32_t pcnt = 0;
            idx_t temp = k;
            while (temp > 1) {
                uint32_t p = (uint32_t)global_spf[temp];
                primes[pcnt++] = p;
                while (temp % p == 0) temp /= p;
            }

            seq_t not_coprime = 0;
            uint32_t subsets = (1u << pcnt);
            for (uint32_t mask = 1; mask < subsets; ++mask) {
                idx_t prod = 1;
                uint32_t bits = 0;
                for (uint32_t j = 0; j < pcnt; ++j) {
                    if (mask & (1u << j)) {
                        bits++;
                        prod *= primes[j];
                        if (prod > r) break;   /* contribution will be zero */
                    }
                }
                if (prod > r) continue;
                seq_t add = r / prod;
                if (bits & 1u) not_coprime += add;
                else           not_coprime -= add;
            }
            R = r - not_coprime;
        }

        seq_t left = (seq_t)q * phi_k + R;
        if (left == (seq_t)k) ans++;
    }
    return ans;
}
