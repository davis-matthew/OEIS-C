/*
 * A213423  Number of partitions of n in which all parts are >= 2 and the
 *          largest part occurs at least four times.
 * https://oeis.org/A213423
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p(n) - 2*p(n-1) + p(n-3) + p(n-4) - 2*p(n-6) + p(n-7)
 * where p(n) is the ordinary partition function (number of ways to write
 * n as a sum of positive integers, ignoring order).  The recurrence holds
 * for all n ≥ 8.  For n < 8 the function returns 0 (offset 8).
 *
 * The algorithm pre‑computes partition numbers up to the requested n
 * using the classic additive dynamic programming:
 *   p[0] = 1
 *   for k = 1 .. n
 *       for s = k .. n
 *           p[s] += p[s-k]
 *
 * Only integer arithmetic on type seq_t is used, so the function works
 * for 8‑, 16‑, 32‑, 64‑, 128‑bit unsigned integers, and for arbitrary
 * precision seq_t provided by the test harness.
 */

#include <stdlib.h>   /* for malloc, free */
#include <stddef.h>   /* for size_t */

static seq_t *p_arr = NULL;   /* partition numbers p[0..max] */
static idx_t  p_max = 0;      /* highest index computed so far */

/* Ensure that partition numbers up to n are available.  The array is
 * recomputed from scratch each time it grows because that keeps the
 * implementation simple and correct for all supported seq_t types. */
static void
ensure_partitions(idx_t n)
{
    if (n <= p_max)
        return;

    /* Allocate new array */
    seq_t *new_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_arr)
        return; /* allocation failed; caller will get zeroed values */

    /* Initialize to zero (seq_t can be assigned from 0) */
    for (idx_t i = 0; i <= n; ++i)
        new_arr[i] = 0;

    /* Classical DP: partitions of 0 is 1 */
    new_arr[0] = 1;

    /* Compute all partition numbers up to n */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = k; s <= n; ++s) {
            new_arr[s] += new_arr[s - k];
        }
    }

    /* Free old array if present */
    if (p_arr)
        free(p_arr);

    p_arr = new_arr;
    p_max = n;
}

seq_t
a(idx_t n)
{
    if (n < 8)
        return 0;   /* offset 8 */

    /* Make sure all required partition numbers are available */
    ensure_partitions(n);

    seq_t pos = 0;   /* terms that are added */
    seq_t neg = 0;   /* terms that are subtracted */

    pos += p_arr[n];           /* p(n)      */

    if (n >= 3)
        pos += p_arr[n - 3];   /* p(n-3)    */
    if (n >= 4)
        pos += p_arr[n - 4];   /* p(n-4)    */
    if (n >= 7)
        pos += p_arr[n - 7];   /* p(n-7)    */

    if (n >= 1) {
        seq_t tmp = p_arr[n - 1];
        neg += tmp;          /* -p(n-1)   */
        neg += tmp;          /* -p(n-1)   */
    }
    if (n >= 6) {
        seq_t tmp = p_arr[n - 6];
        neg += tmp;          /* -p(n-6)   */
        neg += tmp;          /* -p(n-6)   */
    }

    /* pos ≥ neg for all n ≥ 8, subtraction is safe for unsigned type */
    return pos - neg;
}
