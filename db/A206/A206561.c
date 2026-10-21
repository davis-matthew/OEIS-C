/*
 * A206561  Triangle read by rows: T(n,k) = total sum of parts >= k in all
 *          partitions of n.
 * https://oeis.org/A206561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with T(r,k) = Σ_{i=k}^r i * Σ_{j≥1} p(r-j*i),
 *            where p(m) is the partition number of m. r,k are the row and
 *            column of the flattened index n
 * Range:     u8 n <= 36, u16 n <= 351, u32 n <= 3916, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for partition numbers */
static seq_t *P = NULL;       /* array of p[0..] */
static idx_t P_cap = 0;       /* number of computed partition entries (max n) */

/* compute partition numbers up to 'n' (inclusive) using DP. */
static void ensure_partitions(idx_t n)
{
    if (n <= P_cap) return;          /* already available */

    /* allocate new array */
    seq_t *new_arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_arr) return;            /* allocation failed, leave undefined */

    /* initialize */
    for (idx_t i = 0; i <= n; ++i) new_arr[i] = 0;
    new_arr[0] = 1;

    /* classic DP: for each part size, add its contributions */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            new_arr[sum] += new_arr[sum - part];
        }
    }

    /* replace old array */
    free(P);
    P = new_arr;
    P_cap = n;
}

/* locate row r and column k for flattened index n. */
static void row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t low = 1, high = n, r = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    *row = r;
    idx_t prev_tri = (r - 1) * r / 2;
    *col = n - prev_tri;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* outside offset – trivial guard */

    idx_t r, k;
    row_col(n, &r, &k);

    ensure_partitions(r);

    seq_t total = 0;
    for (idx_t i = k; i <= r; ++i) {
        seq_t part_sum = 0;
        /* sum over j≥1 of P[r - j*i] */
        for (idx_t rem = r - i; ; rem -= i) {
            part_sum += P[rem];
            if (rem < i) break;
        }
        total += (seq_t)i * part_sum;
    }

    return total;
}
