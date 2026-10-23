/*
 * A194552  Sum of all parts > 1 of all partitions of n.
 * https://oeis.org/A194552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) gives the total sum of all parts greater than 1 in all
 *            unrestricted partitions of n. The value is computed by dynamic
 *            programming over the partition generating function. Complexity:
 *            O(n²) time and O(n) memory for each n computed
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 89, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *part = NULL;
static seq_t *sumParts = NULL;
static idx_t computed_max = 0;

/* Compute the partition numbers and the required sums up to index n.
   If n <= computed_max the tables are already available. */
static void compute_upto(idx_t n)
{
    if (n <= computed_max)
        return;

    idx_t newsize = n + 1;

    seq_t *new_part = NULL;
    seq_t *new_sum  = NULL;

#ifdef __cplusplus
    try {
        new_part = new seq_t[newsize];
        new_sum  = new seq_t[newsize];
    } catch (...) {
        abort();
    }
    std::memset(new_part, 0, newsize * sizeof(seq_t));
    std::memset(new_sum,  0, newsize * sizeof(seq_t));
#else
    new_part = (seq_t *)calloc(newsize, sizeof(seq_t));
    new_sum  = (seq_t *)calloc(newsize, sizeof(seq_t));
    if (!new_part || !new_sum)
        abort();
#endif

    new_part[0]  = (seq_t)1;
    new_sum[0]   = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {
            for (idx_t m = 1; m <= n; ++m) {
                new_part[m]    += new_part[m - 1];
                new_sum[m]     += new_sum[m - 1];
            }
        } else {
            for (idx_t m = k; m <= n; ++m) {
                new_part[m]    += new_part[m - k];
                new_sum[m]     += new_sum[m - k] + (seq_t)k * new_part[m - k];
            }
        }
    }

    /* Free old tables */
    if (part && sumParts) {
#ifdef __cplusplus
        delete[] part;
        delete[] sumParts;
#else
        free(part);
        free(sumParts);
#endif
    }

    part  = new_part;
    sumParts = new_sum;
    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    compute_upto(n);
    return sumParts[n];
}
