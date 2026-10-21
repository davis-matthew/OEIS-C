/*
 * A204277  Number of (n+2)X3 0..2 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally exactly three ways, and new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A204277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204277: Number of (n+2)×3 0..2 arrays with every 3×3 subblock
   having exactly three equal elements in a line (horizontal, vertical,
   diagonal or antidiagonal).  Computed by the empirical recurrence
   repeatable for n > 16:
     a(n) = 4 a(n-1) –3 a(n-2) +10 a(n-3) –44 a(n-4) +60 a(n-5)
            –30 a(n-6) –172 a(n-7) –152 a(n-8) +1224 a(n-9)
            –448 a(n-10) –656 a(n-11) +880 a(n-12) –2592 a(n-13)
            +2688 a(n-14) –768 a(n-15)
   Initial values: a(1)=108, a(2)=274, a(3)=783, a(4)=1758,
   a(5)=5263, a(6)=14141, a(7)=29731, a(8)=93809,
   a(9)=248919, a(10)=573623, a(11)=1749927,
   a(12)=4584571, a(13)=10749287, a(14)=31949583,
   a(15)=84592247, a(16)=205786143.  */

#include <stdlib.h>  /* malloc, realloc, free */

/* static cache for computed terms; grows as needed */
static seq_t *cache = NULL;          /* 1‑based array: cache[n] = a(n) */
static idx_t max_cached = 0;         /* largest n for which cache[n] is valid */

/* initial 16 terms (index 0 unused) */
static const seq_t init[17] = { 0,
    108, 274, 783, 1758, 5263, 14141, 29731, 93809,
    248919, 573623, 1749927, 4584571, 10749287,
    31949583, 84592247, 205786143
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    /* ensure cache is large enough */
    if (n > max_cached) {
        /* grow cache */
        idx_t new_size = n + 1;            /* 1‑based */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache = new_cache;

        /* fill in missing values */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 16) {
                cache[i] = init[i];
            } else {
                /* compute using the 15‑term recurrence with unsigned arithmetic */
                seq_t pos = 0;
                pos += cache[i-1] * 4;
                pos += cache[i-3] * 10;
                pos += cache[i-5] * 60;
                pos += cache[i-9] * 1224;
                pos += cache[i-12] * 880;
                pos += cache[i-14] * 2688;

                seq_t neg = 0;
                neg += cache[i-2] * 3;
                neg += cache[i-4] * 44;
                neg += cache[i-6] * 30;
                neg += cache[i-7] * 172;
                neg += cache[i-8] * 152;
                neg += cache[i-10] * 448;
                neg += cache[i-11] * 656;
                neg += cache[i-13] * 2592;
                neg += cache[i-15] * 768;

                cache[i] = pos - neg;
            }
        }
        max_cached = n;
    }

    return cache[n];
}
