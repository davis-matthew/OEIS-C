/*
 * A216232  Square array A, read by antidiagonals: A(n,k) = 0 if n-k >= 3 or
 *          if k-n >= 5, A(2,0) = A(1,0) = A(0,0) = A(0,1) = A(0,2) = A(0,3) =
 *          A(0,4) = 1, A(n,k) = A(n-1,k) + A(n,k-1).
 * https://oeis.org/A216232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value A(n,k) of the square array read by *
 *            anti‑diagonals. * Definition of A(n,k): * A(n,k) = 0 if n-k >= 3
 *            or k-n >= 5 * A(0,0) … A(0,4) = 1 * A(1,0) = A(2,0) = 1 * A(n,k)
 *            = A(n-1,k) + A(n,k-1) otherwise * Sequence a(n) = A(n_i,k_i)
 *            where (n_i,k_i) satisfies * n_i + k_i = s (anti‑dia...
 * Range:     u8 n <= 69, u16 n <= 218, u32 n <= 758, u64 n <= 2810,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing a(0), a(1), … */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* two buffers holding values for the previous and current anti‑diagonal */
    #define MAX_DIAG 200   /* enough for n up to ≥10000 */
    static seq_t buf0[MAX_DIAG];
    static seq_t buf1[MAX_DIAG];
    static seq_t *prev_buf = buf0;
    static seq_t *cur_buf  = buf1;
    static idx_t current_diag = 0;

    /* initial allocation for cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* generate anti‑diagonals until we have a(n) */
    while (cache_len <= n) {
        idx_t s = current_diag;                     /* s = n + k on this diagonal */

        for (idx_t i = 0; i <= s; ++i) {
            idx_t k   = s - i;
            int diff  = (int)i - (int)k;           /* n - k */

            seq_t val;
            if (diff >= 3 || diff <= -5) {
                /* outside band: zero */
                val = 0;
            } else if ((i == 0 && k <= 4) ||               /* A(0,0..4) = 1 */
                       (i == 1 && k == 0) ||               /* A(1,0) = 1 */
                       (i == 2 && k == 0)) {               /* A(2,0) = 1 */
                val = 1;
            } else {
                /* recurrence A(i,k) = A(i-1,k) + A(i,k-1) */
                seq_t left = (i > 0) ? prev_buf[i-1] : 0;
                seq_t up   = (i <= s-1) ? prev_buf[i] : 0;
                val = left + up;
            }

            cur_buf[i] = val;  /* store for later use */

            /* append to cache, enlarging it if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        /* move to the next diagonal */
        seq_t *tmp_buf = prev_buf;
        prev_buf = cur_buf;
        cur_buf = tmp_buf;
        ++current_diag;
    }

    return cache[n];
}
