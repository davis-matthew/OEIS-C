/*
 * A028476  Greatest k such that phi(k) = phi(n), where phi is Euler's totient
 *          function.
 * https://oeis.org/A028476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 2; for n > 2, a(n) = A057826(phi(n)) where
 *            A057826(m) is the greatest k with phi(k) = m
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   brute_best_k, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes φ(k) for k up to a fixed limit
   (5 000 000).  For any n whose φ(n) ≤ this limit it can answer in
   O(1).  An a(1) or a(2) normalisation guarantees the correct
   behaviour for the special case φ = 1.  The function is safe for
   any number of calls in arbitrary order. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi.h"   /* contains static seq_t phi(idx_t m) */
#include "../utils/brute_best_k.h"

#define MAX_LIMIT 5000000ULL   /* pre‑compute φ up to this size */

static uint64_t *phi_tbl   = NULL;   /* φ[i] for 0 … MAX_LIMIT */
static uint64_t *best_k_tbl = NULL;  /* best (max) k for every φ value ≤ MAX_LIMIT */
static int initialized = 0;

/* initialise the tables once --------------------------------------- */
static void init_tables(void)
{
    size_t n = (size_t)MAX_LIMIT;
    phi_tbl    = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    best_k_tbl = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!phi_tbl || !best_k_tbl) {
        /* If allocation fails we abort – this should never happen in
           typical test environments. */
        exit(1);
    }

    /* φ[i] = i initially.  best_k values are zero. */
    for (size_t i = 0; i <= n; ++i) {
        phi_tbl[i]    = (uint64_t)i;
        best_k_tbl[i] = 0ULL;
    }

    /* sieve for φ */
    for (size_t p = 2; p <= n; ++p) {
        if (phi_tbl[p] == p) {        /* p is prime */
            for (size_t k = p; k <= n; k += p) {
                phi_tbl[k] -= phi_tbl[k] / p;
            }
        }
    }

    /* compute best_k_tbl: for each φ value remember the largest k */
    for (size_t k = 1; k <= n; ++k) {
        uint64_t m = phi_tbl[k];
        if (m <= n && k > best_k_tbl[m]) {
            best_k_tbl[m] = k;
        }
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    /* n is unsigned 64‑bit.  For n == 1 or 2 the sequence gives 2. */
    if (n <= 2ULL) return (seq_t)2ULL;

    /* compute φ(n) */
    seq_t phi_val_seq = phi((idx_t)n);    /* φ is defined in phi.h */
    uint64_t phi_val  = (uint64_t)phi_val_seq; /* fits in 64‑bit */

    /* If φ(n) is 1 (occurs only for n = 1,2) we also return 2. */
    if (phi_val == 1ULL) return (seq_t)2ULL;

    if (phi_val <= MAX_LIMIT) {
        /* best_k_tbl[phi_val] guaranteed to be non‑zero for φ = 1 as well. */
        uint64_t best = best_k_tbl[phi_val];
        return (seq_t)best;
    }

    /* fallback – unlikely to be reached for n ≤ 10 000 */
    return brute_best_k(phi_val);
}
