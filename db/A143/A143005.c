/*
 * A143005  a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+25)*a(n) - n^6*a(n-1).
 * https://oeis.org/A143005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1 a(n+1) = (2*n+1)*(n^2+n+25)*a(n) - n^6*a(n-1)
 *            (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   mul_seqseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_seqseq.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0; /* next free index */

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n)
        return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; abort (no runtime I/O allowed). */
        abort();
    }
    cache = new_cache;

    /* Initialize new entries if needed. */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
    }
    if (new_size >= 2) {
        cache[1] = (seq_t)1;
    }
    /* Compute terms for indices >= cache_size. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) continue; /* already set */
        if (i == 1) continue; /* already set */
        /* i corresponds to a(i). Use recurrence with n = i-1. */
        idx_t m = i - 1;
        seq_t sm = (seq_t)m;
        seq_t two_sm_plus1 = (sm << 1) + (seq_t)1;

        seq_t m2 = mul_seqseq(sm, sm);                     /* m^2 */
        seq_t m3 = mul_seqseq(m2, sm);                    /* m^3 */
        seq_t m6 = mul_seqseq(m3, m3);                    /* m^6 */

        seq_t coeff_part = m2 + sm + (seq_t)25;           /* m^2 + m + 25 */
        seq_t coeff = mul_seqseq(two_sm_plus1, coeff_part); /* (2m+1)*(m^2+m+25) */

        seq_t part1 = mul_seqseq(coeff, cache[i - 1]);     /* coeff * a(i-1) */
        seq_t part2 = mul_seqseq(m6, cache[i - 2]);        /* m^6 * a(i-2) */

        cache[i] = part1 - part2;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
