/*
 * A158159  Number of nondecreasing integer sequences of length 25 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{k=0}^{25} p_exact(n,k) * Σ_{m=0}^{25-k} p_exact(n,m)
 *            * where p_exact(n,k) is the number of partitions of n into
 *            exactly k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 44, u64 n <= 172, u128 n <= 263,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Mutable cache of partitions into exactly k parts, k ≤ 25 */
static seq_t *p_exact = NULL;          /* flat array of size (max_n+1)*26 */
static idx_t  p_exact_max_n = 0;       /* highest n that has been computed */

/* Ensure that the partition table is filled for all n ≤ limit. */
static void ensure_partitions(idx_t limit)
{
    const idx_t K = 26;                /* columns: k = 0..25 */

    /* Lazily allocate the initial row 0 */
    if (p_exact == NULL) {
        size_t sz = K;
        p_exact = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!p_exact)
            abort();
        p_exact[0] = 1;                /* p_exact(0,0) = 1 */
        p_exact_max_n = 0;
    }

    if (limit <= p_exact_max_n)
        return;

    /* Grow the table to accommodate rows up to 'limit' */
    idx_t old_rows = p_exact_max_n + 1;
    idx_t new_rows = limit + 1;
    size_t new_size = (size_t)new_rows * K;

    seq_t *tmp = (seq_t *)realloc(p_exact, new_size * sizeof(seq_t));
    if (!tmp)
        abort();
    p_exact = tmp;

    /* Zero the newly added rows */
    size_t new_bytes = (size_t)(new_rows - old_rows) * K * sizeof(seq_t);
    if (new_bytes)
        memset(p_exact + (size_t)old_rows * K, 0, new_bytes);

    /* Compute rows from old_rows to limit */
    for (idx_t i = old_rows; i <= limit; ++i) {
        for (idx_t k = 0; k < K; ++k) {
            if (i == 0)
                continue;                /* row 0 already set */

            seq_t val = 0;
            if (k != 0) {
                /* p_exact(i,k) = p_exact(i-1,k-1) + p_exact(i-k,k) */
                if (i >= 1) {
                    size_t idx1 = ((size_t)(i - 1) * K) + (k - 1);
                    val = p_exact[idx1];
                }
                if (i >= k) {
                    size_t idx2 = ((size_t)(i - k) * K) + k;
                    val += p_exact[idx2];
                }
            }
            p_exact[(size_t)i * K + k] = val;
        }
    }

    p_exact_max_n = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset 1 – undefined for n=0 */

    ensure_partitions(n);

    /* p_exact(n,k) values for k=0..25 */
    const idx_t K = 26;
    seq_t prefix[K];
    prefix[0] = p_exact[(size_t)n * K];
    for (idx_t k = 1; k < K; ++k)
        prefix[k] = prefix[k - 1] + p_exact[(size_t)n * K + k];

    seq_t result = 0;
    for (idx_t k = 0; k < K; ++k) {
        seq_t term  = p_exact[(size_t)n * K + k];
        seq_t mult  = prefix[25 - k];
        result += term * mult;
    }
    return result;
}
