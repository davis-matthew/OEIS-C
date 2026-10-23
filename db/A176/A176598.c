/*
 * A176598  Numbers n such that phi(n)/n = 16/41.
 * https://oeis.org/A176598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 410 * s(n) where s(n) is the n-th 2–5–41 smooth number
 *            (product of powers of 2, 5 and 41)
 * Range:     u8 none, u16 n <= 19, u32 n <= 279, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Store the generated 2‑5‑41 smooth numbers (s(1)=1). */
static seq_t *smooth = NULL;          /* array of smooth numbers         */
static idx_t  smooth_size = 0;        /* number of elements stored        */
static idx_t  idx2 = 0, idx5 = 0, idx41 = 0;  /* next indices for *2, *5, *41 */

/* Ensure the cache contains at least `need` values (need ≥ 1). */
static void extend_cache(idx_t need)
{
    if (need <= smooth_size)
        return;

    /* Allocate or grow the smooth array. */
    seq_t *tmp = (smooth == NULL)
        ? (seq_t *)malloc(need * sizeof(seq_t))
        : (seq_t *)realloc(smooth, need * sizeof(seq_t));
    if (!tmp)
        abort();                 /* memory exhausted */

    /* IMPORTANT: update the global pointer before accessing it. */
    smooth = tmp;

    /* First time initialization. */
    if (smooth_size == 0) {
        smooth[0] = (seq_t)1;
        idx2 = idx5 = idx41 = 0;
        smooth_size = 1;
    }

    /* Generate smooth numbers up to need‑1. */
    for (idx_t i = smooth_size; i < need; ++i) {
        seq_t cand2  = smooth[idx2] * (seq_t)2;
        seq_t cand5  = smooth[idx5] * (seq_t)5;
        seq_t cand41 = smooth[idx41] * (seq_t)41;
        seq_t val = cand2;
        if (cand5 < val) val = cand5;
        if (cand41 < val) val = cand41;
        smooth[i] = val;

        /* Advance indices that produced the chosen value. */
        if (val == cand2) ++idx2;
        if (val == cand5) ++idx5;
        if (val == cand41) ++idx41;
    }

    smooth_size = need;
}

/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n is 1‑based (n >= 1).  Defensive guard for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have generated enough smooth numbers. */
    extend_cache(n);

    /* Multiply the smooth number by the constant base 410. */
    const seq_t base = (seq_t)410;
    return base * smooth[n - 1];
}
