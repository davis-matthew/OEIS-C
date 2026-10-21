/*
 * A068577  Numbers k such that tau(k)^2 > phi(k).
 * https://oeis.org/A068577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 396, u32 n <= 396, u64 n <= 396,
 *            u128 n <= 396, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number k such that τ(k)^2 > φ(k) (OEIS A068577) via
   dynamic sieving of smallest prime factors and factorisation. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;          /* smallest prime factor table */
static idx_t     spf_size = 0;        /* current maximum value indexed */

/* rebuilds spf array up to at least 'limit' (inclusive) */
static void rebuild_spf(idx_t limit) {
    if (limit <= spf_size) return;

    idx_t new_size = limit + 1;       /* allocate up to limit */
    uint32_t *new_spf = (uint32_t *)calloc(new_size, sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes (spf version) */
    for (idx_t i = 2; i < new_size; ++i) {
        if (new_spf[i] == 0) {               /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= new_size / i) {          /* avoid overflow */
                for (idx_t j = i * i; j < new_size; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_size = new_size;
}

/* computes τ(k) and φ(k) given that spf covers k */
static void tau_phi(idx_t k, uint64_t *tau_out, uint64_t *phi_out) {
    uint64_t tau = 1;
    uint64_t phi = k;
    idx_t tmp = k;

    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        tau *= (exp + 1);
        phi = phi / p * (p - 1);
    }
    *tau_out = tau;
    *phi_out = phi;
}

/* public function required by the build */
seq_t a(idx_t n) {
    static idx_t current_limit = 2;      /* start checking from 2 */
    size_t found = 0;
    idx_t k = 2;

    /* ensure spf table is ready */
    rebuild_spf(current_limit);

    while (1) {
        if (k >= current_limit) {
            current_limit = k + 100;          /* grow window a bit */
            rebuild_spf(current_limit);
        }

        uint64_t tau, phi;
        tau_phi(k, &tau, &phi);
        if (tau * tau > phi) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
