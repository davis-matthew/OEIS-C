/*
 * A072209  Number of primitive roots of those integers with at least one
 *          primitive root.
 * https://oeis.org/A072209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = φ(φ(A033948(n))) (primitive‑root indicator)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* choose a sieve limit that covers the expected range of n */
/* For n up to about 10 000 this is more than enough */
#define MAX_LIMIT   500000ULL

static bool initialized = false;
static idx_t *prim_roots = NULL;  /* list of numbers with a primitive root */
static size_t prim_roots_count = 0;
static seq_t *phi_arr      = NULL;   /* Euler's totient values 0..MAX_LIMIT */

/* ---------------------------------------------------------------------- */
/* Helper: linear sieve to compute primes and φ up to MAX_LIMIT           */
/* ---------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;

    /* allocate tables */
    phi_arr = (seq_t *)malloc((MAX_LIMIT + 1) * sizeof(seq_t));
    if (!phi_arr) return;   /* out of memory */

    /* spf array to aid the sieve (not strictly needed for φ but useful) */
    uint32_t *spf = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) { free(phi_arr); return; }

    uint32_t *primes = (uint32_t *)malloc((MAX_LIMIT / 10) * sizeof(uint32_t));
    if (!primes) { free(phi_arr); free(spf); return; }
    size_t primes_cnt = 0;

    spf[0] = spf[1] = 0;
    phi_arr[0] = 0;
    phi_arr[1] = 1;

    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i]     = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
            phi_arr[i] = (seq_t)i - 1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t v = i * (idx_t)p;
            if (v > MAX_LIMIT) break;
            spf[v] = p;
            if (i % p == 0) {
                phi_arr[v] = phi_arr[i] * (seq_t)p;
                break;
            } else {
                phi_arr[v] = phi_arr[i] * (seq_t)(p - 1);
            }
        }
    }

    /* mark all numbers that admit a primitive root */
    uint8_t *present = (uint8_t *)calloc((MAX_LIMIT + 1), sizeof(uint8_t));
    if (!present) { free(phi_arr); free(spf); free(primes); return; }

    /* special cases */
    if (MAX_LIMIT >= 1) present[1] = 1;
    if (MAX_LIMIT >= 2) present[2] = 1;
    if (MAX_LIMIT >= 4) present[4] = 1;

    /* for every odd prime p generate p^k and 2*p^k */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p == 2) continue;               /* skip 2 */

        seq_t cur = p;
        while (cur <= MAX_LIMIT) {
            present[(idx_t)cur] = 1;
            if (cur <= MAX_LIMIT / 2) {
                present[(idx_t)(cur * 2)] = 1;
            }
            if (cur > MAX_LIMIT / p) break;
            cur *= (seq_t)p;
        }
    }

    /* collect into the list in increasing order */
    prim_roots = (idx_t *)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    if (!prim_roots) {
        free(phi_arr); free(spf); free(primes); free(present);
        return;
    }

    for (idx_t i = 1; i <= MAX_LIMIT; ++i) {
        if (present[i]) {
            prim_roots[prim_roots_count++] = i;
        }
    }

    /* free temporary storage */
    free(spf);
    free(primes);
    free(present);

    initialized = true;
}

/* ---------------------------------------------------------------------- */
/* Public function required by the test harness                          */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, safety only */
    init_sequence();

    if (n - 1 >= prim_roots_count) return 0;   /* out of range */

    idx_t m = prim_roots[n - 1];
    seq_t phi_m = phi_arr[m];
    seq_t phi_phi_m = phi_arr[(idx_t)phi_m];
    return phi_phi_m;
}
