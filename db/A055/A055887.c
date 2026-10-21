/*
 * A055887  Number of ordered partitions of partitions.
 * https://oeis.org/A055887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
 *               a(n) = Σ_{k=1..n} p(k)*a(n-k)   (n ≥ 1)
 * where p(k) is the number of integer partitions of k.
 * This implementation memoises both the partition numbers p(k)
 * and the sequence terms a(n) for every n requested. */

#include <stdlib.h>
#include <string.h>   /* for memset */

seq_t a(idx_t n) {
    /* static memoisation state */
    static seq_t *p = NULL;          /* partition numbers p[0..maxN]   */
    static seq_t *a_arr = NULL;     /* sequence values a[0..maxN]    */
    static idx_t maxN = 0;           /* largest n already computed    */

    /* initialise for the first-ever call */
    if (maxN == 0 && n == 0) {
        p = (seq_t *)malloc(sizeof(seq_t));
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!p || !a_arr) exit(1);
        p[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
        maxN = 0;
        return a_arr[0];
    }

    /* expand storage if a newer n is requested */
    if (n > maxN) {
        idx_t newSize = n + 1;

        /* grow the partition array */
        seq_t *new_p = (seq_t *)realloc(p, newSize * sizeof(seq_t));
        if (!new_p) exit(1);
        p = new_p;

        /* grow the sequence array */
        seq_t *new_a = (seq_t *)realloc(a_arr, newSize * sizeof(seq_t));
        if (!new_a) exit(1);
        a_arr = new_a;

        /* reset partition numbers and recompute them up to n */
        memset(p, 0, newSize * sizeof(seq_t));
        p[0] = (seq_t)1;                     /* p(0) = 1 */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; ++j) {
                p[j] += p[j - i];
            }
        }

        /* compute a(n) for all new indices */
        for (idx_t i = maxN + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += p[k] * a_arr[i - k];
            }
            a_arr[i] = sum;
        }

        maxN = n;
    }

    return a_arr[n];
}
