/*
 * A008048  Coordination sequence T2 for Zeolite Code BIK.
 * https://oeis.org/A008048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2*a(n-2) + 2*a(n-3) + 4*a(n-5) + a(n-6)
   - a(n-8) - 4*a(n-9) - 2*a(n-11) + 2*a(n-12) + a(n-14)
   (initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=26, a(4)=42,
    a(5)=66, a(6)=102, a(7)=140, a(8)=164, a(9)=216, a(10)=288,
    a(11)=334, a(12)=372, a(13)=458, a(14)=556) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached values of the sequence.  The array is expanded on demand. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of values presently stored (>=1 when cache != NULL) */

    /* Allocate and initialise the cache on the first call. */
    if (cache == NULL) {
        cache_len = 15;  /* indices 0 .. 14 */

        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)12;
        cache[3]  = (seq_t)26;
        cache[4]  = (seq_t)42;
        cache[5]  = (seq_t)66;
        cache[6]  = (seq_t)102;
        cache[7]  = (seq_t)140;
        cache[8]  = (seq_t)164;
        cache[9]  = (seq_t)216;
        cache[10] = (seq_t)288;
        cache[11] = (seq_t)334;
        cache[12] = (seq_t)372;
        cache[13] = (seq_t)458;
        cache[14] = (seq_t)556;
    }

    /* Return the result if it is already cached. */
    if (n < cache_len) return cache[n];

    /* Need to compute terms up to index n.  Preserve the previous cache length. */
    const idx_t old_len = cache_len;
    const idx_t new_len = n + 1;

    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Coefficients c0 .. c14 of the denominator polynomial. */
    static const int coef[15] = {
        -1, /* c0 */
        0,  /* c1 */
        -2, /* c2 */
        2,  /* c3 */
        0,  /* c4 */
        4,  /* c5 */
        1,  /* c6 */
        0,  /* c7 */
        -1, /* c8 */
        -4, /* c9 */
        0,  /* c10 */
        -2, /* c11 */
        2,  /* c12 */
        0,  /* c13 */
        1   /* c14 */
    };

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = old_len; i < new_len; ++i) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;
        for (int k = 1; k <= 14; ++k) {
            seq_t val = cache[i - k];
            int c = coef[k];
            if (c > 0) {
                sumPos += val * (seq_t)c;
            } else if (c < 0) {
                sumNeg += val * (seq_t)(-c);
            }
        }
        cache[i] = sumPos - sumNeg;
    }

    return cache[n];
}
