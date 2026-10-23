/*
 * A008291  Triangle of rencontres numbers.
 * https://oeis.org/A008291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 79, u64 n <= 191,
 *            u128 n <= 562, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) enumerates the triangle of rencontres numbers row-wise.
 * For n ≥ 2: let m = n − 2,
 *            r = the unique integer such that r(r−1)/2 > m
 *            k = m − (r−2)(r−1)/2   (0 ≤ k ≤ r−2)
 * then a(n) = binomial(r, k) × D(r−k),
 * where D(i) is the subfactorial (derangement number) with D(0)=1, D(1)=0,
 * D(i) = (i−1)(D(i−1)+D(i−2)) for i≥2.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *der_cache = NULL;    /* cache of derangements D[0..] */
static idx_t der_size = 0;         /* current size of der_cache */

/* Ensure der_cache contains D[0..max_needed] */
static void ensure_der_upto(idx_t max_needed)
{
    if (max_needed < der_size)
        return;

    idx_t new_size = max_needed + 1;
    seq_t *tmp = (seq_t *)realloc(der_cache, new_size * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failed; further calls may produce garbage */

    der_cache = tmp;

    /* initialise missing entries */
    if (der_size == 0) {
        der_cache[0] = 1;    /* D(0) = 1 */
        der_size = 1;
    }
    if (der_size == 1 && max_needed >= 1) {
        der_cache[1] = 0;    /* D(1) = 0 */
        der_size = 2;
    }

    for (idx_t i = der_size; i <= max_needed; ++i) {
        /* D(i) = (i-1) * (D(i-1)+D(i-2)) */
        der_cache[i] = (seq_t)(i - 1) * (der_cache[i - 1] + der_cache[i - 2]);
    }
    der_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2; return 0 */

    /* index m (zero-based) */
    idx_t m = n - 2;

    /* Find r such that r(r-1)/2 > m, using binary search with 128-bit arithmetic */
    idx_t low = 2, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 <= m) {
        high <<= 1;               /* double until high big enough */
    }
    while (high - low > 1) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid - 1) / 2 <= m)
            low = mid;
        else
            high = mid;
    }
    idx_t r = high;   /* row number */

    /* number of entries before this row */
    idx_t C_prev = (unsigned __int128)(r - 2) * (r - 1) / 2;
    idx_t k = m - C_prev;   /* column index (0 ≤ k ≤ r-2) */

    /* ensure we have derangements up to r-k */
    ensure_der_upto(r - k);

    seq_t bin = binomial(r, k);
    seq_t der = der_cache[r - k];
    return bin * der;
}
