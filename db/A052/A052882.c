/*
 * A052882  A simple grammar: rooted ordered set partitions.
 * https://oeis.org/A052882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*A000670(n-1) for n>0, with a(0)=0. A000670(k) = number
 *            of ordered set partitions of k elements
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *F = NULL;      /* F[i] holds A000670(i)  (i >= 0) */
    static idx_t  cur_len = 0;   /* number of computed entries: 0..cur_len-1 */
    static size_t cap    = 0;    /* current allocation size */

    /* ensure F[0..max_k] is available */
    if (n == 0) return (seq_t)0;
    idx_t max_k = n - 1;

    if (cur_len <= max_k) {
        size_t needed = max_k + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap << 1 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(F, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);   /* allocation failed */
            F = tmp;
            cap = new_cap;
        }
        for (idx_t i = cur_len; i <= max_k; ++i) {
            if (i == 0) {
                F[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                seq_t bin = (seq_t)1;          /* binomial(i,0) */
                for (idx_t k = 0; k < i; ++k) {
                    sum += bin * F[k];
                    bin = bin * (i - k) / (k + 1);  /* bin = binomial(i,k+1) */
                }
                F[i] = sum;
            }
        }
        cur_len = max_k + 1;
    }

    /* a(n) = n * F[n-1] */
    return (seq_t)n * F[max_k];
}
