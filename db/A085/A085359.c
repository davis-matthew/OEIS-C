/*
 * A085359  Third self-convolution of A085357 (residues of
 *          binomial(3k,k)/(2k+1) modulo 2).
 * https://oeis.org/A085359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (i,j,k) of non‑negative
 *            integers * with i+j+k = n such that each of i, j, k has no
 *            consecutive 1‑bits in its * binary representation (equivalently,
 *            (binom(3i,i) / (2i+1)) mod 2 = 1, etc.)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *A        = NULL;  /* a(n) values up to cachedMax          */
static seq_t *pairCnt  = NULL;  /* number of ordered pairs (i,j) with i+j = m */
static uint8_t *b      = NULL;  /* 1 if k has no consecutive 1‑bits         */
static idx_t *ones    = NULL;   /* indices k where b[k] == 1                 */
static size_t   onesCnt = 0;     /* number of such indices                 */
static idx_t   cachedMax = 0;    /* highest n for which arrays are valid   */

/* Allocate or reallocate arrays to hold values up to newMax (inclusive). */
static void build_up_to(idx_t newMax)
{
    size_t sz = (size_t)newMax + 1;
    seq_t *newA    = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newPC   = (seq_t*)calloc(sz, sizeof(seq_t));
    uint8_t *newB   = (uint8_t*)calloc(sz, sizeof(uint8_t));
    idx_t *newOnes  = (idx_t*)calloc(sz, sizeof(idx_t));

    /* Build b[k] and collect indices where the condition holds. */
    onesCnt = 0;
    for (idx_t k = 0; k <= newMax; ++k) {
        uint8_t bk = ((k & (k << 1)) == 0) ? 1 : 0;  /* no consecutive 1s */
        newB[(size_t)k] = bk;
        if (bk) newOnes[onesCnt++] = k;
    }

    /* Compute ordered pair counts: number of (i,j) with i+j = s. */
    for (size_t ii = 0; ii < onesCnt; ++ii) {
        idx_t i = newOnes[ii];
        for (size_t jj = 0; jj < onesCnt; ++jj) {
            idx_t j  = newOnes[jj];
            idx_t sum = i + j;
            if (sum <= newMax) newPC[(size_t)sum] += (seq_t)1;
        }
    }

    /* Compute a[n] as convolution of pairCnt with the set of indices. */
    for (size_t kk = 0; kk < onesCnt; ++kk) {
        idx_t k = newOnes[kk];
        idx_t maxRest = newMax - k;
        for (idx_t s = 0; s <= maxRest; ++s) {
            newA[(size_t)(s + k)] += newPC[(size_t)s];
        }
    }

    /* Replace old arrays. */
    free(A);
    free(pairCnt);
    free(b);
    free(ones);

    A        = newA;
    pairCnt  = newPC;
    b        = newB;
    ones     = newOnes;
    cachedMax = newMax;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (A == NULL || n > cachedMax) build_up_to(n);
    return A[(size_t)n];
}
