/*
 * A039896  Number of partitions satisfying 0 < cn(1,5) + cn(4,5) + cn(2,5) +
 *          cn(3,5).
 * https://oeis.org/A039896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - (n mod 5 == 0)*p(n/5)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(n) is the partition function. */
#include <stdlib.h>

/* Static cache for partition numbers. */
static seq_t *partition = NULL;
static idx_t par_size = 0;

/* Compute partition numbers p[0..N] and store them in p. */
static void compute_partitions(idx_t N, seq_t *p)
{
    p[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            /* sign = +1 for odd k, -1 for even k */
            int sign = (k & 1) ? 1 : -1;
            if (sign > 0)
                sum += p[n - g1];
            else
                sum -= p[n - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= n) {
                if (sign > 0)
                    sum += p[n - g2];
                else
                    sum -= p[n - g2];
            }
        }
        p[n] = sum;
    }
}

/* Return the n-th term of the sequence A039896. */
seq_t a(idx_t n)
{
    /* Extend the cached partitions if needed. */
    if (par_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_parts = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_parts)
            return 0;               /* Allocation failure -> zero */
        compute_partitions(n, new_parts);
        free(partition);
        partition = new_parts;
        par_size = new_size;
    }

    seq_t res = partition[n];
    if ((n % 5) == 0)
        res -= partition[n / 5];
    return res;
}
