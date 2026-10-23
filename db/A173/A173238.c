/*
 * A173238  Triangle by columns, A000041 in every column shifted down twice
 *          for columns > 0.
 * https://oeis.org/A173238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = A000041(r - 2k) where r is the row of the triangle
 *            T(m,k)=A000041(m-2k) that contains the index n, and k is the
 *            column offset within that row. The triangle is flattened
 *            row‑major to form the sequence
 * Range:     u8 n <= 80, u16 n <= 505, u32 n <= 4159, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of partition numbers p[0..N] */
static seq_t *part_cache = NULL;
static idx_t part_cap   = 0;

/* Compute all partition numbers up to max_n and store them in part_cache. */
static void ensure_partitions(idx_t max_n)
{
    if (part_cap > max_n) return;                 /* already sufficient */

    /* Allocate new cache */
    seq_t *new_cache = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
    if (!new_cache) abort();

    /* Work in signed 64‑bit to avoid intermediate underflow/overflow. */
    long long *tmp = (long long *)malloc((max_n + 1) * sizeof(long long));
    if (!tmp) { free(new_cache); abort(); }

    tmp[0] = 1;
    for (idx_t n = 1; n <= max_n; ++n) {
        long long sum = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)(k * (3 * k - 1) / 2);
            if (pent1 > n) break;
            long long sign = (k & 1) ? 1 : -1;
            sum += sign * tmp[n - pent1];
            idx_t pent2 = (idx_t)(k * (3 * k + 1) / 2);
            if (pent2 <= n) sum += sign * tmp[n - pent2];
        }
        tmp[n] = sum;
    }

    for (idx_t i = 0; i <= max_n; ++i) new_cache[i] = (seq_t)tmp[i];

    free(tmp);
    free(part_cache);
    part_cache = new_cache;
    part_cap   = max_n;
}

/* Main sequence function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r that contains the flattened index n.
       Row lengths are floor(r/2)+1. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (1) {
        idx_t row_len = (r >> 1) + 1;            /* floor(r/2)+1 */
        if (cum + row_len > n) break;
        cum += row_len;
        ++r;
    }

    idx_t k = n - cum;                         /* column offset within row */
    idx_t j = r - 2 * k;                       /* corresponding partition index */

    ensure_partitions(j);
    return part_cache[j];
}
