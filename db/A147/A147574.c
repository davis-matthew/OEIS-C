/*
 * A147574  Numbers with exactly 7 distinct prime divisors {2,3,5,7,11,13,17}.
 * https://oeis.org/A147574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 510510 * A080681(n)
 * Range:     u8 none, u16 none, u32 n <= 868, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The allowed primes */
static const seq_t PRIMES[7] = { 2, 3, 5, 7, 11, 13, 17 };

/* State kept between calls */
static seq_t *smooth   = NULL; /* 7‑smooth numbers: 1,2,3,4,…            */
static size_t smooth_cap = 0;   /* current allocated capacity              */
static size_t smooth_len = 0;   /* number of values stored                 */
static size_t idxs[7] = { 0 };  /* indices for the 7 primes                */

/* Ensure smooth has capacity at least 'need'.  */
static void ensure_capacity(size_t need)
{
    if (smooth_cap < need) {
        size_t new_cap = smooth_cap ? smooth_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
        smooth = tmp;
        smooth_cap = new_cap;
    }
}

/* Return the n-th term of A147574 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* for safety, although offset = 1 */
    size_t target = (size_t)(n - 1);  /* zero‑based index */

    /* initialise the cache on the first call */
    if (smooth == NULL) {
        ensure_capacity(16);
        smooth[0] = 1;               /* 1 is the first 7‑smooth number */
        smooth_len = 1;
        for (size_t i = 0; i < 7; ++i) idxs[i] = 0;
    }

    /* generate 7‑smooth numbers until we reach the requested one */
    while (smooth_len <= target) {
        /* grow the array if we are about to write past the end */
        if (smooth_len >= smooth_cap) ensure_capacity(smooth_len + 1);

        seq_t next = 0;                   /* initialise to zero */
        for (size_t i = 0; i < 7; ++i) {
            seq_t cand = PRIMES[i] * smooth[idxs[i]];
            if (next == 0 || cand < next) next = cand;
        }

        smooth[smooth_len++] = next;

        /* advance any indices that produced this minimum */
        for (size_t i = 0; i < 7; ++i) {
            seq_t cand = PRIMES[i] * smooth[idxs[i]];
            if (cand == next) ++idxs[i];
        }
    }

    /* Each term is 510510 multiplied by the corresponding 7‑smooth number */
    return (seq_t)510510 * smooth[target];
}
