/*
 * A026800  Number of partitions of n in which the least part is 7.
 * https://oeis.org/A026800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n-7) - p(n-8) - p(n-9) + p(n-12) + 2*p(n-14) - p(n-16)
 *            - p(n-17) - p(n-18) - p(n-19) + 2*p(n-21) + p(n-23) - p(n-26) -
 *            p(n-27) + p(n-28) where p(k) = A000041(k) and p(k)=0 for k<0
 * Range:     u8 n <= 54, u16 n <= 104, u32 n <= 241, u64 n <= 648,
 *            u128 n <= 1962, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of partition numbers p(0..p_max).  The array is grown lazily. */
static seq_t *partitions = NULL;
static idx_t    p_max     = 0;

/* Build or extend the table of partition numbers up to n. */
static void build_partitions(idx_t n)
{
    if (n <= p_max) return;                /* already computed */

    /* Allocate or enlarge the array. */
    seq_t *newp = (seq_t *)realloc(partitions,
                                   (size_t)(n + 1) * sizeof(seq_t));
    if (!newp) return;                     /* allocation failure – ignore */
    partitions = newp;

    /* Initialize new entries to zero.  p[0] will always be 1. */
    idx_t start = (p_max == 0) ? 1 : p_max + 1;
    for (idx_t i = start; i <= n; ++i)
        partitions[i] = (seq_t)0;

    /* p(0) = 1 */
    if (p_max == 0) partitions[0] = (seq_t)1;

    /* Compute partition numbers for indices > p_max. */
    for (idx_t m = p_max + 1; m <= n; ++m) {
        seq_t pos = (seq_t)0; /* sum of terms with + sign   */
        seq_t neg = (seq_t)0; /* sum of terms with - sign   */

        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;
            if (k & 1)   pos += partitions[m - pent1];
            else         neg += partitions[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m) continue;
            if (k & 1)   pos += partitions[m - pent2];
            else         neg += partitions[m - pent2];
        }
        partitions[m] = pos - neg;  /* pos >= neg for all m */
    }
    p_max = n;
}

/* Return a(n) – number of partitions of n whose least part is 7. */
seq_t a(idx_t n)
{
    /* Ensure partition numbers up to n are available. */
    build_partitions(n);

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    if (n >= 7)  pos += partitions[n - 7];
    if (n >= 8)  neg += partitions[n - 8];
    if (n >= 9)  neg += partitions[n - 9];
    if (n >= 12) pos += partitions[n - 12];
    if (n >= 14) { pos += partitions[n - 14]; pos += partitions[n - 14]; }
    if (n >= 16) neg += partitions[n - 16];
    if (n >= 17) neg += partitions[n - 17];
    if (n >= 18) neg += partitions[n - 18];
    if (n >= 19) neg += partitions[n - 19];
    if (n >= 21) { pos += partitions[n - 21]; pos += partitions[n - 21]; }
    if (n >= 23) pos += partitions[n - 23];
    if (n >= 26) neg += partitions[n - 26];
    if (n >= 27) neg += partitions[n - 27];
    if (n >= 28) pos += partitions[n - 28];

    return pos - neg;     /* pos >= neg for all n */
}
