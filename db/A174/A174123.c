/*
 * A174123  Partial sums of A002893.
 * https://oeis.org/A174123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*  A174123: a(n) = Sum_{i=0..n} A002893(i)
    Recurrence (see OEIS entry):
      9*(n+2)^2*a(n) - (99+86*n+19*n^2)*a(n+1)
      + (72+56*n+11*n^2)*a(n+2) - (n+3)^2*a(n+3) = 0
    Initial values: a(0)=1, a(1)=4, a(2)=19 */
static seq_t *cache     = NULL;  /* Cached computed terms */
static idx_t cache_len  = 0;     /* Number of terms currently cached */

/* Expand the cache so that terms 0..n are computed */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                 /* already computed */

    /* Grow the array */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();         /* allocation failure */
    cache = tmp;

    /* Compute missing terms */
    for (idx_t k = cache_len; k < new_len; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)4;
        } else if (k == 2) {
            cache[2] = (seq_t)19;
        } else {
            /* Derive a(k) from a(k-3), a(k-2), a(k-1) using the recurrence */
            idx_t nrec = k - 3;   /* n in the recurrence */
            /* Coefficients */
            idx_t coeff1 = 99 + 86 * nrec + 19 * nrec * nrec;
            idx_t coeff2 = 72 + 56 * nrec + 11 * nrec * nrec;

            if (sizeof(seq_t) <= 8) {            /* 64‑bit or smaller: use 128‑bit ints for safety */
                unsigned __int128 a0 = (unsigned __int128)cache[k - 3];
                unsigned __int128 a1 = (unsigned __int128)cache[k - 2];
                unsigned __int128 a2 = (unsigned __int128)cache[k - 1];

                unsigned __int128 term0 = (unsigned __int128)9 *
                                          (unsigned __int128)(nrec + 2) *
                                          (unsigned __int128)(nrec + 2) * a0;
                unsigned __int128 term1 = (unsigned __int128)coeff1 * a1;
                unsigned __int128 term2 = (unsigned __int128)coeff2 * a2;

                unsigned __int128 numerator   = term0 - term1 + term2;
                unsigned __int128 denominator = (unsigned __int128)(k) * (unsigned __int128)(k);
                cache[k] = (seq_t)(numerator / denominator);
            } else {                            /* 128‑bit or arbitrary‑precision: use seq_t arithmetic */
                seq_t term0 = (seq_t)9 *
                              (seq_t)(nrec + 2) *
                              (seq_t)(nrec + 2) *
                              cache[k - 3];
                seq_t term1 = (seq_t)coeff1 * cache[k - 2];
                seq_t term2 = (seq_t)coeff2 * cache[k - 1];

                seq_t numerator   = term0 - term1 + term2;
                seq_t denominator = (seq_t)k * (seq_t)k;
                cache[k] = numerator / denominator;
            }
        }
    }
    cache_len = new_len;
}

/* Required public function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
