/*
 * A217301  Numbers n such that gcd(n, phi(n)) = gcd(phi(n), sigma(n)) =
 *          gcd(sigma(n), n) = tau(n).
 * https://oeis.org/A217301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implement the OEIS sequence A217301: 
 * Numbers n such that gcd(n, φ(n)) = gcd(φ(n), σ(n)) = gcd(σ(n), n) = τ(n).
 *
 * Algorithm:
 *   • Build a smallest‑prime‑factor (SPF) array up to the current sieve limit
 *     using a classic linear sieve.
 *   • For each integer n > last_processed, obtain its prime factorisation via
 *     the SPF array.  From the factorisation compute φ(n), σ(n) and τ(n)
 *     using 64‑bit unsigned arithmetic.
 *   • Test whether the four values are pairwise equal.
 *   • Store every qualifying n in a dynamic array.
 *   • When the caller requests the k‑th term a(k), expand the sieve (doubling
 *     the limit each time) until at least k qualifying numbers have been
 *     found, then return the k‑th value (cast to seq_t).
 *
 * The algorithm is O(N log log N) for sieving and remains fast for n up to
 * ~10 000, satisfying the time requirement.  All intermediate arithmetic
 * uses uint64_t so that the result is exact for any seq_t size; the final
 * value is cast to seq_t on return.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t     spf_limit = 0;      /* current SPF array maximum */

static uint64_t *found = NULL;       /* stored qualifying numbers (full value) */
static size_t    found_len = 0;       /* number of stored values   */
static size_t    found_cap = 0;       /* array capacity           */

static uint64_t  last_processed = 0;  /* highest n that has been examined */

/* ------------------------------------------------------------------- */
/* Utility: compute the greatest common divisor of two 64‑bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------- */
/* Build or extend the SPF array up to ‘limit’ (inclusive).            */
static void build_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) return;  /* out of memory – leave previous SPF unchanged */

    for (size_t i = 0; i <= limit; ++i) {
        new_spf[i] = (uint32_t)i;
    }

    /* simple Eratosthenes sieve to fill SPF */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (new_spf[i] == i) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                if (new_spf[j] == j) new_spf[j] = i;
            }
        }
    }

    /* free old array and replace */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ------------------------------------------------------------------- */
/* For a given n, compute φ(n), σ(n) and τ(n).  All functions return
 * 64‑bit results.                                                            */
static void compute_phi_sigma_tau(uint64_t n,
                                  uint64_t *phi,
                                  uint64_t *sigma,
                                  uint64_t *tau)
{
    uint64_t n_copy = n;
    uint64_t phi_val = n;
    uint64_t sigma_val = 1;
    uint64_t tau_val = 1;

    while (n_copy > 1) {
        uint32_t p = spf[n_copy];
        uint32_t exp = 0;
        uint64_t power = 1;
        uint64_t sum   = 1;   /* 1 + p + p^2 + ... + p^exp */

        while (n_copy % p == 0) {
            n_copy /= p;
            ++exp;
            power *= (uint64_t)p;
            sum += power;
        }

        /* φ(n): φ -= φ / p  */
        phi_val -= phi_val / (uint64_t)p;

        tau_val   *= (uint64_t)(exp + 1);
        sigma_val *= sum;
    }

    *phi   = phi_val;
    *sigma = sigma_val;
    *tau   = tau_val;
}

/* ------------------------------------------------------------------- */
/* Test all integers from last_processed+1 up to ‘limit’.                */
static void process_up_to(size_t limit)
{
    if (limit <= last_processed) return;

    if (limit > spf_limit) build_spf(limit);

    for (uint64_t n = last_processed + 1; n <= limit; ++n) {
        uint64_t phi, sigma, tau;
        compute_phi_sigma_tau(n, &phi, &sigma, &tau);

        uint64_t g = gcd_u64(n, phi);
        if (g == gcd_u64(phi, sigma) &&
            g == gcd_u64(sigma, n) &&
            g == tau) {
            /* store the qualifying n */
            if (found_len == found_cap) {
                size_t new_cap = found_cap ? found_cap * 2u : 4u;
                uint64_t *tmp = (uint64_t *)realloc(found, new_cap * sizeof(uint64_t));
                if (!tmp) return;          /* out of memory – abort incrementally */
                found = tmp;
                found_cap = new_cap;
            }
            found[found_len++] = n;
        }
    }

    last_processed = limit;
}

/* ------------------------------------------------------------------- */
/* Public function required by the test harness.                        */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but return 0 */

    /* initialise storage if first call */
    if (!found) {
        found_cap = 4;
        found = (uint64_t *)malloc(found_cap * sizeof(uint64_t));
    }

    /* generate numbers until we have at least n qualifying terms */
    while (found_len < (size_t)n) {
        size_t next_limit = last_processed ? last_processed * 2u : 1000u;
        process_up_to(next_limit);
        /* if still not enough, loop again – spf will grow progressively */
    }

    return (seq_t)found[n - 1];
}
