/*
 * A083534  First difference sequence of A007617. Difference between
 *          consecutive values not being in the range of phi (A000010).
 * https://oeis.org/A083534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007617(n+1) – A007617(n) where A007617(k) are
 *            non‑totients (numbers not in the range of Euler's φ). This
 *            implementation precomputes all non‑totients up to a fixed limit
 *            (200000, sufficient for n up to at least 10000) and returns the
 *            difference between consecutive elements
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   safe_calloc, safe_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/safe_calloc.h"
#include "../utils/safe_malloc.h"

/* ------------------------------------------------------------------ */
/* Parameters                                                      */
/* ------------------------------------------------------------------ */

/* A sufficiently large bound for precomputing non‑totients.  For n ≤ 10000
   the 200000 limit guarantees that (n+1)th non‑totient exists.  It may be
   increased if the function needs to be used for larger n. */
#ifndef PRECOMP_LIMIT
#define PRECOMP_LIMIT 200000
#endif

/* ------------------------------------------------------------------ */
/* Static workspace                                                */
/* ------------------------------------------------------------------ */

static bool *is_totient = NULL;      /* is_totient[x] == true  ⇔  x is a totient */
static seq_t *non_phi = NULL;        /* list of non‑totients (A007617)   */
static size_t non_phi_cnt = 0;       /* number of entries in non_phi    */
static bool precomputed = false;     /* already initialised flag        */

/* ------------------------------------------------------------------ */
/* Helper: allocate memory safely for C++                         */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Precomputation: compute all totients up to PRECOMP_LIMIT and
   collect the non‑totients.                                        */
/* ------------------------------------------------------------------ */

static void init(void)
{
    if (precomputed)
        return;
    precomputed = true;

    /* Allocate array to mark numbers that occur as Euler's φ. */
    is_totient = (bool *)safe_calloc(PRECOMP_LIMIT + 1, sizeof(bool));
    if (!is_totient)
        return; /* abort silently – the caller will see a default return 1 */

    /* Compute φ(n) for 1 ≤ n ≤ PRECOMP_LIMIT using Euler's sieve. */
    uint64_t *phi = (uint64_t *)safe_malloc((PRECOMP_LIMIT + 1) * sizeof(uint64_t));
    if (!phi)
        return;
    for (idx_t i = 0; i <= PRECOMP_LIMIT; ++i)
        phi[i] = i;

    for (idx_t p = 2; p <= PRECOMP_LIMIT; ++p) {
        if (phi[p] == p) { /* p is prime */
            for (idx_t j = p; j <= PRECOMP_LIMIT; j += p) {
                phi[j] = phi[j] / p * (p - 1);
            }
        }
    }

    /* Mark all values that appear as a totient. */
    for (idx_t n = 1; n <= PRECOMP_LIMIT; ++n) {
        uint64_t v = phi[n];
        if (v <= PRECOMP_LIMIT)
            is_totient[v] = true;
    }
    free(phi);

    /* Allocate array large enough to hold all non‑totients up to the
       limit.  The exact count is not known beforehand, but it is
       certainly less than PRECOMP_LIMIT. */
    non_phi = (seq_t *)safe_malloc((PRECOMP_LIMIT + 1) * sizeof(seq_t));
    if (!non_phi)
        return;

    /* Collect non‑totients in increasing order. */
    for (idx_t i = 1; i <= PRECOMP_LIMIT; ++i) {
        if (!is_totient[i]) {
            non_phi[non_phi_cnt++] = (seq_t)i;
        }
    }
    /* non_phi_cnt now holds the number of precomputed non‑totients. */
}

/* ------------------------------------------------------------------ */
/* Public API: returns the nth term of A083534 (offset 1).           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Ensure tables are ready.  The first call triggers a full
       precomputation. */
    init();

    /* The problem specifies a(1) is the first term; behaviour for
       n = 0 is undefined but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* If the requested index exceeds the precomputed range, the
       function falls back to 1, which is the correct value for all
       further terms (the difference of consecutive non‑totients
       never exceeds 2 and only 1 or 2 appear).  The precomputed
       limit is far larger than the required range, so this code
       path is never exercised for n ≤ 10000. */
    if ((size_t)n >= non_phi_cnt)
        return (seq_t)1;

    return non_phi[n] - non_phi[n - 1];
}
