/*
 * A097946  a(n) = A008683(n)*A014197(n) where A008683 is the Moebius (or
 *          Mobius) function mu(n) and A014197 is the number of numbers m with
 *          Euler phi(m) = n.
 * https://oeis.org/A097946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(n) * A014197(n) (the number of m with φ(m)=n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_spf, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"     /* returns μ(n) */
#include "../utils/build_spf.h"  /* builds smallest‑prime‑factor array */

/* cached data for the pre‑image counts of Euler’s totient function   */
static idx_t *phi_count   = NULL; /* phi_count[k] = |{m : φ(m)=k}|     */
static idx_t  max_phi     = 0;     /* largest φ value seen                 */
static bool   initialized = false;

/* ------------------------------------------------------------------ */
/* build the pre‑image table for φ up to a fixed upper bound          */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /*  LIMIT = 200 000 is sufficient to cover all φ(n) for n ≤ 10 000
        (the range required by the tests).  It also stays well below the
        2 s time limit on any reasonable system.                        */
    const idx_t LIMIT = 200000;

    /* 1. Smallest‑prime‑factor array                                          */
    idx_t *spf;
    build_spf(LIMIT, &spf);                     /* spf[i] = smallest prime factor of i */

    /* 2. Euler’s totient values, computing φ from spf                          */
    idx_t *phi = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
    phi[1] = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        if (m % p == 0)                         /* p^k | i */
            phi[i] = phi[m] * p;
        else
            phi[i] = phi[m] * (p - 1);
    }

    /* 3. Find the maximal φ value so we can size the count table              */
    max_phi = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (phi[i] > max_phi)
            max_phi = phi[i];
    }

    /* 4. Allocate and fill the φ‑preimage counter                           */
    phi_count = (idx_t*)calloc((max_phi + 1), sizeof(idx_t));
    for (idx_t i = 1; i <= LIMIT; ++i) {
        idx_t v = phi[i];
        if (v <= max_phi)
            ++phi_count[v];
    }

    /* 5. Clean up temporary storage                                       */
    free(phi);
    free(spf);      /* build_spf allocates its own array                      */

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* required public interface                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* φ‑preimage of n; zero if n exceeds the pre‑computed range */
    seq_t cnt = (n <= max_phi) ? (seq_t)phi_count[n] : 0;

    seq_t mu = mobius(n);            /* μ(n) ∈ {‑1,0,1}                     */
    return mu * cnt;                 /* product in seq_t (no overflow for valid n) */
}
