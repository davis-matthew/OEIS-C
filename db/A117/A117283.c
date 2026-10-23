/*
 * A117283  Numbers k for which the cototient k-phi(k) is a cube.
 * https://oeis.org/A117283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 4244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117283: numbers k such that k-phi(k) is a perfect cube.
   Computation: generate k from 1 upward, compute φ(k) with calc_phi,
   and include k if k-φ(k) is a cube. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/floor_cube_root.h"

static seq_t *terms = NULL;       /* cached sequence values              */
static size_t terms_len = 0;       /* number of values stored             */
static size_t terms_cap = 0;       /* allocated capacity                  */
static idx_t next_k = 1;           /* next k to examine                   */

/* Ensure the sequence array has room for at least `n` elements. */
static void ensure_terms_capacity(size_t n)
{
    if (n <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) { /* handle allocation failure gracefully */
        return;
    }
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Fill the cache until we have at least n terms. */
    while (terms_len < n) {
        seq_t k_val = (seq_t)next_k;
        seq_t phi_val = calc_phi(k_val);      /* φ(k) */
        seq_t diff = k_val - phi_val;         /* k - φ(k) */
        seq_t c = floor_cube_root(diff);      /* ⌊∛(k-φ(k))⌋ */
        if (c * c * c == diff) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = k_val;
        }
        ++next_k;
    }
    return terms[n - 1];
}
