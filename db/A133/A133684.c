/*
 * A133684  a(2n) = A001045(n); a(1)=1; a(2n+1) = 2*A001045(n-1) for n >= 1.
 * https://oeis.org/A133684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2n) = A001045(n), a(2n+1) = 2*A001045(n-1)
 *            (for n ≥ 1)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 67, u64 n <= 131,
 *            u128 n <= 259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2n) is the n‑th Jacobsthal number J(n) (J(0)=0, J(1)=1, J(k)=J(k‑1)+2*J(k‑2)). */

#include <stddef.h>
#include <stdlib.h>

/* Cached Jacobsthal numbers */
static seq_t *jacob_cache = NULL;
static size_t  jacob_cap   = 0;   /* allocated capacity in seq_t elements */
static idx_t   jacob_len   = 0;   /* number of Jacobsthal numbers currently cached */

/* Ensure J(0)…J(n) are present in the cache (n inclusive). */
static void ensure_jacob(idx_t n)
{
    /* Fast return if already cached. */
    if (n < jacob_len)
        return;

    /* Initialise if this is the first call. */
    if (jacob_cache == NULL) {
        /* At least two elements are needed for J(0) and J(1). */
        jacob_cap   = 4;
        jacob_cache = (seq_t *)malloc(jacob_cap * sizeof(seq_t));
        if (!jacob_cache) { /* allocation failure: abort the program – the build framework handles it */
            abort();
        }
        jacob_cache[0] = (seq_t)0;   /* J(0) */
        jacob_cache[1] = (seq_t)1;   /* J(1) */
        jacob_len     = 2;
    }

    /* Grow the cache if necessary to hold up to n */
    while (jacob_len <= n) {
        if (jacob_len >= jacob_cap) {
            size_t new_cap = jacob_cap * 2;
            seq_t *tmp = (seq_t *)realloc(jacob_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            jacob_cache = tmp;
            jacob_cap   = new_cap;
        }
        /* J(k) = J(k-1) + 2*J(k-2) */
        seq_t val = jacob_cache[jacob_len - 1] + ((seq_t)2 * jacob_cache[jacob_len - 2]);
        jacob_cache[jacob_len] = val;
        jacob_len++;
    }
}

/* Sequence A133684 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if ((n & 1) == 0) {              /* n = 2k : a(2k) = J(k) */
        idx_t k = n >> 1;
        ensure_jacob(k);
        return jacob_cache[k];
    } else {                          /* n = 2k+1 (k ≥ 1) : a(2k+1) = 2*J(k-1) */
        idx_t k = (n - 1) >> 1;      /* k ≥ 1 */
        ensure_jacob(k - 1);
        return (seq_t)2 * jacob_cache[k - 1];
    }
}
