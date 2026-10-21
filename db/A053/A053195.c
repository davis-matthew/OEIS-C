/*
 * A053195  Number of level permutations of degree n.
 * https://oeis.org/A053195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053195: Number of level permutations of degree n.
   a(n) = sum_{k=0}^{v2(n)} f[n][k]
   where f[n][k] counts permutations of n whose all cycle lengths have the same 2‑adic valuation k.
   f[0][k] = 1, and for n > 0,
   f[n][k] = Σ_{l=1..n, v2(l)=k} ( (n-1)! / (n-l)! ) * f[n-l][k]   (in exact integer arithmetic). */

#include <stdlib.h>

/* forward declarations for the build's integer types */
extern idx_t trailing_zeros(idx_t);   /* defined below */

/* Compute trailing zero count for idx_t that is non‑zero */
static inline idx_t v2(idx_t x) {
    /* simple bit scan for portable C */
    idx_t c = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

/* Dynamic storage for f[n][k] */
static seq_t *f = NULL;           /* flattened 2‑D array */
static idx_t f_nmax = 0;          /* maximum n currently stored */
static idx_t f_kmax = 0;          /* maximum k currently stored (max v2) */

/* Ensure the table is filled up to Nmax */
static void ensure_capacity(idx_t Nmax) {
    if (Nmax <= f_nmax && Nmax <= f_nmax) return;   /* already ready */

    /* new maximal k for the requested Nmax */
    idx_t newKmax = 0;
    idx_t tmp = Nmax;
    while (tmp > 1) {
        tmp >>= 1;
        ++newKmax;
    }

    /* allocate new storage */
    size_t newSize = (size_t)(Nmax + 1) * (newKmax + 1);
    seq_t *newF = (seq_t*)calloc(newSize, sizeof(seq_t));
    if (!newF) abort();            /* out of memory */

    /* copy old data */
    if (f) {
        for (idx_t n = 0; n <= f_nmax; ++n) {
            for (idx_t k = 0; k <= f_kmax; ++k) {
                newF[n * (newKmax + 1) + k] = f[n * (f_kmax + 1) + k];
            }
        }
        free(f);
    }

    /* initialise base cases for new rows/cols */
    for (idx_t k = 0; k <= newKmax; ++k)
        newF[0 * (newKmax + 1) + k] = 1;   /* f[0][k] = 1 */

    /* compute DP for new rows */
    for (idx_t n = f_nmax + 1; n <= Nmax; ++n) {
        seq_t currT = 1;                       /* T[1] */
        for (idx_t l = 1; l <= n; ++l) {
            idx_t k = v2(l);
            /* Add contribution from a cycle of length l */
            seq_t add = currT * newF[(n - l) * (newKmax + 1) + k];
            newF[n * (newKmax + 1) + k] += add;
            if (l < n)
                currT = currT * (seq_t)(n - l);
        }
    }

    /* replace old table */
    f = newF;
    f_nmax = Nmax;
    f_kmax = newKmax;
}

/* Public function required by the interface */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;      /* a(0) = 1 */
    ensure_capacity(n);

    idx_t v = v2(n);
    seq_t result = 0;
    if ((n & 1) == 0) {                 /* even n */
        for (idx_t k = 0; k <= v; ++k) {
            result += f[n * (f_kmax + 1) + k];
        }
    } else {                            /* odd n: only k = 0 contributes */
        result = f[n * (f_kmax + 1) + 0];
    }
    return result;
}
