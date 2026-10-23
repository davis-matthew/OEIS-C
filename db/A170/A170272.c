/*
 * A170272  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170272 – Number of reduced words of length n in a Coxeter group
    with 23 generators S_i, (S_i)^2 = I, (S_i S_j)^41 = I.
    Generating function:
      A(t) = (t^41 + 2t^40 + … + 2t + 1)/(231t^41 – 21t^40 – … – 21t + 1)
    Recurrence (derived from the rational form):
      a(0) = 1
      for 1 ≤ n ≤ 40:
          a(n) = 2 + 21 * Σ_{k=1}^{n} a(n‑k)
      a(41)  = 21 * Σ_{k=1}^{40} a(k) – 230
      for n ≥ 42:
          a(n) = 21 * Σ_{k=1}^{40} a(n‑k) – 231 * a(n‑41)
    The function memoises the sequence so that calls may be in any order
    and the time is O(n) for n up to about 10 000.  */

#include <stdlib.h>

static idx_t cache_size = 0;
static seq_t *cache = NULL;

seq_t a(idx_t n)
{
    /* initialise cache with a(0) = 1 */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();          /* memory allocation failed */
        cache[0] = (seq_t)1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();           /* memory allocation failed */
        cache = tmp;
        cache_size = new_size;

        /* compute new values up to n */
        for (idx_t cur = old_size; cur <= n; cur++) {
            seq_t sum = (seq_t)0;
            /* sum previous at most 40 terms */
            for (idx_t k = 1; k <= 40 && cur >= k; k++)
                sum += cache[cur - k];

            seq_t val;
            if (cur <= 40) {
                val = sum * (seq_t)21 + (seq_t)2;
            } else if (cur == 41) {
                val = sum * (seq_t)21 - (seq_t)230;
            } else {
                val = sum * (seq_t)21 - (seq_t)231 * cache[cur - 41];
            }
            cache[cur] = val;
        }
    }

    return cache[n];
}
