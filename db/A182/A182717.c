/*
 * A182717  Number of 2's in all partitions of 2n+1 that do not contain 1 as a
 *          part.
 * https://oeis.org/A182717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 67, u64 n <= 214, u128 n <= 739,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A182717
    a(n) = number of 2's in all partitions of (2n+1) that do not contain 1.
    Formula: a(n) = sum_{odd k=1}^{2n-1} (p(k) - p(k-1))
             = sum_{i=0}^{2n-2} (-1)^{i+1} p(i)
    where p(m) is the ordinary partition number of m.
    The implementation builds partition numbers up to 2n-1 using
    the generalized pentagonal recurrence and then evaluates the
    alternating sum.  It caches the partition table so that
    subsequent calls reuse already computed values.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* number of partitions needed: 0 .. 2n-1 */
    idx_t m = 2 * n;                     /* m = 2n   */
    static seq_t *p_cache = NULL;        /* array of p[0..m-1] */
    static idx_t cache_size = 0;         /* number of computed entries */

    /* resize cache if necessary */
    if (m > cache_size) {
        /* allocate or grow array */
        seq_t *new_cache = (seq_t *)realloc(p_cache, (size_t)m * sizeof(seq_t));
        if (new_cache == NULL) exit(1);          /* abort on OOM */
        p_cache = new_cache;

        /* initialize p[0] */
        if (cache_size == 0) {
            p_cache[0] = 1;                     /* p(0) = 1 */
            cache_size = 1;
        }

        /* compute missing partition numbers */
        for (idx_t i = cache_size; i < m; ++i) {
            seq_t pos = 0, neg = 0;             /* sums of + and - terms */
            for (idx_t k = 1; ; ++k) {
                /* generalized pentagonal numbers */
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                if (k & 1) pos += p_cache[i - g1];
                else       neg += p_cache[i - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > i) continue;
                if (k & 1) pos += p_cache[i - g2];
                else       neg += p_cache[i - g2];
            }
            p_cache[i] = pos - neg;          /* pos >= neg for all i */
        }
        cache_size = m;
    }

    /* evaluate the alternating sum */
    seq_t sum_even = 0, sum_odd = 0;
    for (idx_t i = 0; i < m; ++i) {
        if ((i & 1) == 0)
            sum_even += p_cache[i];
        else
            sum_odd += p_cache[i];
    }

    return sum_odd - sum_even;              /* non‑negative result */
}
