/*
 * A027336  Number of partitions of n that do not contain 2 as a part.
 * https://oeis.org/A027336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1; for n>=2, a(n)=p(n)-p(n-2), where p(n) is the
 *            partition function. The function uses the partition numbers
 *            computed by a simple dynamic‑programming algorithm. Memory for
 *            the partition array is allocated and freed with the helper
 *            functions alloc_seq_array and free_seq_array, which work...
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* cache of partition numbers up to the highest n requested */
static seq_t *partitions = NULL;      /* p[0 .. max_computed] */
static idx_t max_computed = 0;        /* largest n for which p[n] is known */

/* compute partition numbers up to at least n */
static void ensure_partitions(idx_t n)
{
    if (n <= max_computed)            /* already computed */
        return;

    seq_t *new_part = alloc_seq_array(n + 1);  /* new array, elements default‑constructed */
    new_part[0] = (seq_t)1;            /* p(0)=1 */

    /* compute p(1..n) with the standard nested‑loop DP */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            new_part[j] += new_part[j - i];
        }
    }

    /* discard old cache */
    if (partitions != NULL)
        free_seq_array(partitions);

    partitions     = new_part;
    max_computed   = n;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 2)                /* a(0) = a(1) = 1 */
        return (seq_t)1;

    ensure_partitions(n);     /* p[0..n] are now available */

    return partitions[n] - partitions[n - 2];
}
