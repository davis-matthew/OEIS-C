/*
 * A121081  Number of partitions of n into parts with at most one 1 and at
 *          most one 2.
 * https://oeis.org/A121081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-2) - p(n-4) + p(n-6) where p(k) is the
 *            partition number of k (p(k)=0 for negative k)
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p = NULL;          /* cache of partition numbers p[0..max] */
static idx_t  p_max = 0;         /* highest indexed partition computed */

static void compute_partitions_up_to(idx_t n)
{
    if (p == NULL) {                                 /* initial allocation */
        p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (p == NULL) exit(1);
        p[0] = (seq_t)1;                 /* p(0) = 1 */
    } else if (n > p_max) {                 /* extend existing array */
        seq_t *tmp = (seq_t *)realloc(p, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        p = tmp;
    }

    idx_t start = (p_max == 0 && p != NULL) ? 1 : p_max + 1;
    for (idx_t i = start; i <= n; ++i) {         /* compute p[i] */
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            seq_t term = p[i - g1];
            sum += ((k & 1) ? term : -term);

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i) continue;
            term = p[i - g2];
            sum += ((k & 1) ? term : -term);
        }
        p[i] = sum;
    }
    p_max = (n > p_max) ? n : p_max;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence offset starts at 1 */
    compute_partitions_up_to(n);

    seq_t res = p[n];
    if (n >= 2) res -= p[n - 2];
    if (n >= 4) res -= p[n - 4];
    if (n >= 6) res += p[n - 6];
    return res;
}
