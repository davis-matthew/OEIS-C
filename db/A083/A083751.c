/*
 * A083751  Number of partitions of n into >= 2 parts and with minimum part >=
 *          2.
 * https://oeis.org/A083751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(n-1) - 1, with p(n) = number of partitions of n.
 *            For n ≤ 1, a(n) is defined to be 0
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 151, u64 n <= 465,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* State for a memoised partition table. */
static idx_t max_p = 0;
static seq_t *p  = NULL;

/* Ensure that the partition array contains values up to at least n.  */
static void
ensure_partitions(idx_t n)
{
    if (n <= max_p)
        return;

    idx_t new_size = (max_p == 0) ? 16 : (max_p * 2);
    if (new_size < n + 1)
        new_size = n + 1;

    seq_t *new_p = (seq_t *)realloc(p, new_size * sizeof(seq_t));
    if (!new_p)
        abort(); /* allocation failure */

    p = new_p;

    /* Initialise new entries.  The recurrence below needs p[0] = 1. */
    if (max_p == 0) {
        p[0] = (seq_t)1;    /* base case */
        max_p = 0;
    }

    for (idx_t i = max_p + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i)
                break;
            sum += (k & 1 ? 1 : -1) * p[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i)
                continue;
            sum += (k & 1 ? 1 : -1) * p[i - pent2];
        }
        p[i] = sum;
    }
    max_p = n;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 by definition */

    ensure_partitions(n);

    /* For n ≥ 2, the formula holds.  All intermediate results fit in seq_t. */
    return p[n] - p[n - 1] - 1;
}
