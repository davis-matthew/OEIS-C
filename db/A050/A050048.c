/*
 * A050048  a(n) = a(n-1) + a(m) for n >= 3, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A050048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n >= 3, m = 2*n - 3 - 2^(p+1), p is the
 *            unique integer such that 2^p < n-1 <= 2^(p+1), a(1) = 1, a(2) =
 *            2
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 194, u64 n <= 769,
 *            u128 n <= 3624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *vec = NULL;          /* 1‑based array of terms */
    static idx_t  allocated = 0;       /* current allocated size (>= n+1) */
    static idx_t  len      = 0;        /* maximum n that has been computed */

    if (n < 1) return 0;               /* undefined for n < 1 */

    /* grow the array if necessary */
    if (n + 1 > allocated) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[n + 1];
        if (vec) {
            for (idx_t i = 0; i <= len; ++i) tmp[i] = vec[i];
        }
        delete[] vec;
        vec = tmp;
        allocated = n + 1;
#else
        seq_t *tmp = (seq_t *)realloc(vec, (n + 1) * sizeof(seq_t));
        if (!tmp && n + 1 > 0) abort();
        vec = tmp;
        allocated = n + 1;
#endif
    }

    /* compute any missing terms up to n */
    if (n > len) {
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i == 1) {
                vec[1] = (seq_t)1;
            } else if (i == 2) {
                vec[2] = (seq_t)2;
            } else {
                /* find p with 2^p < i-1 <= 2^(p+1) */
                idx_t x = i - 2;            /* i >= 3 => x >= 1 */
                idx_t p = 0;
                while (x >>= 1) ++p;        /* compute floor(log2(x)) */

                idx_t pow2 = ((idx_t)1) << (p + 1);  /* 2^(p+1) */
                idx_t m = (idx_t)2 * i - 3 - pow2;

                /* recurrence */
                vec[i] = vec[i - 1] + vec[m];
            }
        }
        len = n;
    }

    return vec[n];
}
