/*
 * A158650  Indices of single-digit numbers in A061041.
 * https://oeis.org/A158650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158650: indices k such that A061041(k) < 10
   Here A061041(k) = (k^2 - 16) / (gcd(k-4,32) * gcd(k+4,32))
   Offset is 1 (a(1) = 4, a(2) = 5, …) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *indices   = NULL;          /* stored k values */
static size_t indices_len  = 0;
static size_t indices_cap  = 0;
static idx_t  next_k       = 4;          /* smallest k to test next */

/* Grow the indices array if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= indices_cap) return;
    size_t new_cap = indices_cap ? indices_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(indices, new_cap * sizeof(seq_t));
    if (!tmp) return; /* keep old array on failure */
    indices   = tmp;
    indices_cap = new_cap;
}

/* Generate indices until we have at least 'target' terms */
static void generate_until(size_t target)
{
    if (!indices) {
        indices = (seq_t*)malloc(64 * sizeof(seq_t));
        if (!indices) return;           /* allocation failed */
        indices_cap = 64;
    }

    while (indices_len < target) {
        seq_t k = (seq_t)next_k;

        seq_t km4 = k - 4;                 /* k-4 ≥ 0 */
        seq_t kp4 = k + 4;

        seq_t g1 = gcd(km4, (seq_t)32);
        seq_t g2 = gcd(kp4, (seq_t)32);
        seq_t denom = g1 * g2;             /* denominator (never zero) */

        seq_t num = k * k - (seq_t)16;     /* k^2 - 16  (non‑negative for k≥4) */

        seq_t val = num / denom;           /* integer division truncates */

        if (val < (seq_t)10) {
            ensure_capacity(indices_len + 1);
            if (indices_cap == 0) return;   /* allocation failed */
            indices[indices_len++] = k;
        }

        ++next_k;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0 */
    generate_until((size_t)n);
    if (n <= indices_len)
        return indices[n - 1];
    return 0;                       /* allocation failure or similar */
}
