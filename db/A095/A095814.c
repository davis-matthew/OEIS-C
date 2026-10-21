/*
 * A095814  Number of nonisomorphic partitions of n on the Ferrers diagram.
 * https://oeis.org/A095814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(A000041(n)/2) where A000041(n) is the partition
 *            number of n. The function computes partition numbers by Euler's
 *            pentagonal recurrence and memoises them in a static array
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised partition numbers, p[0] = 1 */
    static seq_t *p = NULL;
    static idx_t size = 0;          /* next free index */

    if (!p) {                       /* first call: allocate p[0] */
        p = (seq_t*)malloc(sizeof(seq_t));
        if (!p) return (seq_t)0;    /* allocation failure */
        p[0] = (seq_t)1;            /* p(0)=1 */
        size = 1;
    }

    if (n >= size) {
        /* extend array to hold up to n */
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(p, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        p = tmp;
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;
                seq_t term = p[i - pent1];
                if (k & 1) sum += term; else sum -= term;
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                if (k & 1) sum += p[i - pent2]; else sum -= p[i - pent2];
            }
            p[i] = sum;
        }
        size = n + 1;
    }

    seq_t pn = p[n];
    seq_t res = pn / (seq_t)2;
    if (pn % (seq_t)2 != (seq_t)0) res += (seq_t)1;   /* ceil division */
    return res;
}
