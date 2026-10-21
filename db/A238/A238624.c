/*
 * A238624  Number of partitions of n such that either both floor(n/2) and
 *          ceiling(n/2) are parts or else neither is a part.
 * https://oeis.org/A238624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A238624: Number of partitions of n such that either both floor(n/2)
   and ceil(n/2) are parts or else neither is a part.
   Formula:
   if n is even         : a(n) = p(n)
   if n is odd (>1)     : a(n) = p(n) - p((n-1)/2) - p((n+1)/2) + 2
   (p(k) = the partition function).
   For n = 1          : a(1) = 0
*/

#include <stdlib.h>

static seq_t *part = NULL;        /* partition numbers cache */
static idx_t max_n = 0;           /* highest n for which part[0..max_n] are valid */

/* Recompute partition numbers up to n (inclusive).  Only called when n > max_n. */
static void
ensure_partitions(idx_t n)
{
    if (n <= max_n)
        return;

    /* Allocate a zeroed array of size n+1 */
    seq_t *new_part = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_part)
        abort();                 /* memory allocation failed */

    new_part[0] = 1;             /* p(0) = 1 */

    /* Standard coin change DP: count partitions */
    for (idx_t coin = 1; coin <= n; ++coin) {
        for (idx_t i = coin; i <= n; ++i) {
            new_part[i] += new_part[i - coin];
        }
    }

    free(part);
    part = new_part;
    max_n = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)        /* behavior below offset irrelevant */
        return 0;

    ensure_partitions(n);

    if ((n & 1) == 0)                 /* even n: all partitions count */
        return part[n];

    if (n == 1) /* the only odd term with special behavior */
        return 0;

    idx_t k = n >> 1;                  /* k = floor(n/2) for odd n */
    seq_t result = part[n];
    result -= part[k + 1];
    result -= part[k];
    result += (seq_t)2;
    return result;
}
