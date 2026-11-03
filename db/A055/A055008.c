/*
 * A055008  Numbers k such that gcd(phi(k), sigma(k)) = 1 with phi = A000010,
 *          sigma = A000203.
 * https://oeis.org/A055008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9448 (2026-11-03)
 * Helpers:   gcd, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055008: numbers k such that gcd(phi(k), sigma(k)) = 1. */
/* The sequence consists only of squares and twice squares, so we generate
   candidates in increasing order by merging these two monotone lists. */
/* A candidate k is accepted if gcd(phi(k), sigma(k)) == 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    /* static storage for the generated sequence */
    static seq_t *seq = NULL;
    static size_t seq_size = 0;
    static size_t seq_cap  = 0;

    /* maintain the next square and twice‑square factors */
    static idx_t sq_s = 1;  /* for squares: k = sq_s^2   */
    static idx_t sq_t = 1;  /* for twice squares: k = 2*sq_t^2 */

    /* generate terms until we have enough */
    while (seq_size < n) {
        seq_t cand_sq  = (seq_t)sq_s * sq_s;
        seq_t cand_tw  = (seq_t)2 * sq_t * sq_t;
        seq_t cand = (cand_sq <= cand_tw) ? cand_sq : cand_tw;

        /* test the condition */
        seq_t phi_k = totient((idx_t)cand);
        seq_t sigma_k = sigma_of((idx_t)cand);
        if (gcd(phi_k, sigma_k) == (seq_t)1) {
            /* store the candidate */
            ensure_capacity(seq_size + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
            seq[seq_size++] = cand;
        }

        /* advance the list that contributed the chosen candidate */
        if (cand == cand_sq)
            sq_s++;
        else
            sq_t++;
    }

    return seq[n - 1];
}
