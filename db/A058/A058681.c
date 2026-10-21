/*
 * A058681  Number of matroids of rank 2 on n labeled points.
 * https://oeis.org/A058681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = B(n+1) - 2^n where B(k) is the k‑th Bell number. This
 *            implements a(n) = B(n+1) - 2^n using the recurrence B(m+1) =
 *            sum_{k=0..m} C(m,k)*B(k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *bell = NULL;       /* array of Bell numbers B[0..] */
static size_t bell_len = 0;      /* number of computed B values (next free index) */
static size_t bell_cap = 0;      /* allocated capacity of bell */

/* Ensure bell has capacity at least `needed`.  Uses realloc. */
static void ensure_bell_capacity(size_t needed)
{
    if (needed <= bell_cap) return;
    size_t newcap = bell_cap ? bell_cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *newptr = (seq_t*)realloc(bell, newcap * sizeof(seq_t));
    assert(newptr); /* abort on allocation failure */
    bell = newptr;
    bell_cap = newcap;
}

/* Compute Bell numbers up to index `idx` (inclusive). */
static void compute_bell_up_to(idx_t idx)
{
    if (idx < (idx_t)bell_len) return;            /* already computed */
    if (bell == NULL) {                           /* first time */
        bell_cap = 8;
        bell = (seq_t*)malloc(bell_cap * sizeof(seq_t));
        assert(bell);
        bell_len = 0;
    }
    if (bell_len == 0) {                          /* initialize B[0] = 1 */
        bell[0] = (seq_t)1;
        bell_len = 1;
    }

    for (idx_t m = (idx_t)bell_len - 1; m < idx; ++m) {
        /* compute B[m+1] = sum_{k=0..m} C(m,k)*B[k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t prod = binomial(m, k) * bell[k];
            sum += prod;
        }
        ensure_bell_capacity((size_t)(m + 2));
        bell[m + 1] = sum;
        ++bell_len;
    }
}

/* Sequence entry function. */
seq_t a(idx_t n)
{
    /* a(0)=0, a(1)=0 are handled by formula automatically */
    /* Compute B(n+1) and 2^n */
    compute_bell_up_to(n + 1);
    seq_t bn1 = bell[n + 1];
    seq_t p2  = pow2(n);      /* 2^n */
    return bn1 - p2;
}
