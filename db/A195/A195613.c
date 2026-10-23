/*
 * A195613  Numbers n with property that n, 2n-1 and 2n+1 are composite.
 * https://oeis.org/A195613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer such that n, 2n−1 and 2n+1 are
 *            all composite
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

/* Cached sequence values */
static seq_t *seq_vals = NULL;   /* array of terms */
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* allocated capacity of seq_vals */
static uint64_t last_checked = 0;/* largest integer examined so far */

/* Ensure the cache has at least 'needed' slots */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;  /* multiply by 2 */
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate terms until we have at least 'upto' terms stored. */
static void generate_until(idx_t upto)
{
    while (seq_len < upto) {
        uint64_t cand = last_checked + 1;
        last_checked = cand;

        seq_t n      = (seq_t)cand;           /* current candidate */
        seq_t n2m1   = (seq_t)(2 * cand - 1); /* candidate - 1 */
        seq_t n2p1   = (seq_t)(2 * cand + 1); /* candidate + 1 */

        /* Check compositeness of n, 2n−1, and 2n+1. */
        if (!is_prime_small(n) &&
            !is_prime_small(n2m1) &&
            !is_prime_small(n2p1)) {
            ensure_capacity(seq_len + 1);
            seq_vals[seq_len++] = n;
        }
    }
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) {               /* undefined for n==0, guard against bad callers */
        return 0;
    }

    generate_until(n);
    return seq_vals[n - 1];
}
