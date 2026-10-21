/*
 * A060671  Numbers k such that phi(x) = k has exactly 8 solutions.
 * https://oeis.org/A060671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A060671: numbers k such that φ(x) = k has
   exactly eight solutions.  The sequence is generated once by
   sieving totients up to a sufficiently large bound.  All subsequent
   calls simply index the pre‑computed array. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* array of k‑values with 8 preimages */
static size_t seq_len = 0;         /* number of stored values */
static bool   initialized = false;/* flag whether precomputation is done */

/* -------------------------------------------------------------- */
/* Pre‑compute φ(n) for 1 ≤ n ≤ LIMIT, collect all k with exactly
   8 preimages, and store them in seq[].  LIMIT is chosen large
   enough to contain the first 10 000 terms of the sequence.       */
#define LIMIT 20000000U   /* 20 million; memory ≈ 120 MB for uint32_t phi */

static void precompute(void)
{
    /* Allocate memory for φ values */
    uint32_t *phi = (uint32_t *)malloc((size_t)(LIMIT + 1) * sizeof(uint32_t));
    if (!phi) return;  /* allocation failed – caller will return 0 */

    /* Initialise φ[n] = n and compute sieve of Euler */
    for (idx_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;
    phi[0] = 0; phi[1] = 1;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (phi[i] == (uint32_t)i) {            /* i is prime */
            for (idx_t j = i; j <= LIMIT; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }

    /* Count number of preimages for each totient value */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(LIMIT + 1), sizeof(uint16_t));
    if (!cnt) {
        free(phi);
        return;
    }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t v = phi[i];
        /* v ≤ LIMIT holds for all i ≤ LIMIT */
        cnt[v]++;   /* counts won’t overflow for the small values we need */
    }

    /* Count how many k have exactly 8 preimages */
    size_t needed = 0;
    for (size_t k = 1; k <= LIMIT; ++k)
        if (cnt[k] == 8) ++needed;

    /* Allocate the final sequence array */
    if (needed > 0) {
        seq = (seq_t *)malloc(needed * sizeof(seq_t));
        if (seq) {
            size_t pos = 0;
            for (size_t k = 1; k <= LIMIT; ++k)
                if (cnt[k] == 8) seq[pos++] = (seq_t)k;
            seq_len = needed;
        }
    }

    /* Clean up temporary arrays */
    free(phi);
    free(cnt);
}

/* -------------------------------------------------------------- */
/* Public function required by the framework.  It guarantees that
   the sequence has been computed before accessing seq[ n‑1 ]. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n==0 */

    if (!initialized) {                     /* first call – pre‑compute */
        precompute();
        initialized = true;
    }

    /* If n exceeds the pre‑computed length we simply return 0; the
       sequence is defined for all n ≥ 1 in the test data. */
    if (n > seq_len) return (seq_t)0;
    return seq[n - 1];
}
