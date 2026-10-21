/*
 * A222705  Total number of parts of multiplicity 5 in all partitions of n.
 * https://oeis.org/A222705
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} p(n-5k) - sum_{k>=1} p(n-6k), where p(m) is
 *            the partition number (p(0)=1, p(m<0)=0)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 139, u64 n <= 426,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for partition numbers */
static seq_t *partitions   = NULL;
static size_t part_capacity = 0;
static idx_t   dp_limit      = 0;

/* compute p(i) using the generalized pentagonal theorem
   and store it in partitions[i]. */
static void compute_partition(idx_t i)
{
    seq_t value = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        if (g1 > i) break;
        if (k & 1) value += partitions[i - g1];
        else       value -= partitions[i - g1];

        idx_t g2 = k * (3 * k + 1) / 2;
        if (g2 > i) continue;
        if (k & 1) value += partitions[i - g2];
        else       value -= partitions[i - g2];
    }
    partitions[i] = value;
}

/* ensure that partitions[0..n] exist, computing missing ones. */
static void ensure_partitions(idx_t n)
{
    if (partitions == NULL) {                     /* first time */
        size_t need = (size_t)(n + 1);
        partitions = (seq_t *)malloc(need * sizeof(seq_t));
        if (!partitions) abort();                 /* allocation failed */
        part_capacity = need;
        partitions[0] = (seq_t)1;
        dp_limit = 0;
    } else if ((size_t)(n + 1) > part_capacity) { /* grow cache */
        size_t need = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(partitions, need * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        partitions = tmp;
        part_capacity = need;
    }
    if (dp_limit < n) {                          /* compute remaining */
        for (idx_t i = dp_limit + 1; i <= n; ++i)
            compute_partition(i);
        dp_limit = n;
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;
    ensure_partitions(n);

    seq_t result = 0;

    /* add contributions from x^5/(1-x^5) */
    for (idx_t k = 1; 5 * k <= n; ++k)
        result += partitions[n - 5 * k];

    /* subtract contributions from x^6/(1-x^6) */
    for (idx_t k = 1; 6 * k <= n; ++k)
        result -= partitions[n - 6 * k];

    return result;
}
