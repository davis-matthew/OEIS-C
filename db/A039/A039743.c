/*
 * A039743  Number k such that gcd(phi(k), k-1) = number of distinct prime
 *          factors of k.
 * https://oeis.org/A039743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search/Sieve: collect all k such that gcd(phi(k),k-1) = ω(k) */
/* The first term corresponds to a(1) = 2.  See OEIS A039743. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/u64_gcd.h"

/* ---------- Utility: gcd for 64‑bit values ---------- */

/* ---------- Global storage for the computed sequence ---------- */
static seq_t *seq_vals = NULL;   /* array of sequence terms       */
static size_t seq_len = 0;       /* number of terms available     */

/* ---------- Initializer: run once, build the sieve ---------- */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Limit large enough to contain enough terms; 6 000 000 yields >10 000 terms. */
    const idx_t LIMIT = 6000000;

    /* Allocate memory for smallest prime factor, phi, and ω(k).  Use C casts for C++. */
    uint32_t *spf   = (uint32_t *)malloc(sizeof(uint32_t) * (LIMIT + 1));
    uint32_t *phi   = (uint32_t *)malloc(sizeof(uint32_t) * (LIMIT + 1));
    uint8_t  *omega = (uint8_t  *)malloc(sizeof(uint8_t)  * (LIMIT + 1));
    if (!spf || !phi || !omega) { abort(); }

    /* ---------- Smallest prime factor sieve ---------- */
    for (idx_t i = 2; i <= LIMIT; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* ---------- Compute φ(k) and ω(k) ---------- */
    phi[1]   = 1;
    omega[1] = 0;

    for (idx_t k = 2; k <= LIMIT; ++k) {
        uint32_t p  = spf[k];
        uint32_t m  = (uint32_t)(k / p);
        if (m % p == 0) {                      /* p^e | k, e>1 */
            phi[k]   = phi[m] * p;
            omega[k] = omega[m];
        } else {                               /* first occurrence of p */
            phi[k]   = phi[m] * (p - 1);
            omega[k] = omega[m] + 1;
        }
    }

    /* ---------- Gather all valid k ---------- */
    const size_t INITIAL_CAP = 32768;
    seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * INITIAL_CAP);
    if (!tmp) abort();

    size_t cap = INITIAL_CAP;
    size_t cnt = 0;

    for (idx_t k = 2; k <= LIMIT; ++k) {
        uint64_t g = u64_gcd((uint64_t)phi[k], (uint64_t)(k - 1));
        if (g == (uint64_t)omega[k]) {
            if (cnt == cap) {
                cap <<= 1;
                seq_t *t = (seq_t *)realloc(tmp, sizeof(seq_t) * cap);
                if (!t) abort();
                tmp = t;
            }
            tmp[cnt++] = (seq_t)k;
        }
    }

    /* ---------- Store the result in the global variables ---------- */
    seq_vals = tmp;
    seq_len  = cnt;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (!seq_vals) init_sequence();          /* build on first use */

    if (n == 0 || n > seq_len) return (seq_t)0;  /* out of range */
    return seq_vals[n - 1];
}
