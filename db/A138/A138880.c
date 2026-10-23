/*
 * A138880  Sum of all parts of all partitions of n that do not contain 1 as a
 *          part.
 * https://oeis.org/A138880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (p(n) – p(n-1)) where p(k) is the partition number of
 *            k (Euler's recurrence)
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 105, u64 n <= 366,
 *            u128 n <= 1344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of partition numbers */
static seq_t *p = NULL;   /* p[0..p_max] */
static idx_t p_max = 0;   /* highest index computed */

/* Ensure that p[0..n] are computed */
static void ensure_partitions(idx_t n)
{
    if (n <= p_max) return;

    /* resize the array */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(p, new_size * sizeof(seq_t));
    if (!tmp) abort();
    p = tmp;

    /* initialise new entries to zero (safe for all seq_t types) */
    if (new_size > (size_t)p_max + 1) {
        for (size_t i = p_max + 1; i < new_size; ++i) p[i] = 0;
    }

    /* make sure p[0] equals 1 */
    if (p_max == 0) p[0] = 1;

    /* compute partition numbers for i = p_max+1 .. n */
    for (idx_t i = p_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)(k * (3 * k - 1) / 2);
            idx_t pent2 = (idx_t)(k * (3 * k + 1) / 2);
            if (pent1 > i && pent2 > i) break;

            if (pent1 <= i) {
                seq_t val = p[i - pent1];
                if (k & 1) pos += val; else neg += val;
            }
            if (pent2 <= i) {
                seq_t val = p[i - pent2];
                if (k & 1) pos += val; else neg += val;
            }
        }
        /* pos ≥ neg for all i, so subtraction is safe */
        p[i] = (pos >= neg) ? (pos - neg) : 0;
    }
    p_max = n;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_partitions(n);
    seq_t diff = p[n] - p[n - 1];
    return diff * (seq_t)n;
}
