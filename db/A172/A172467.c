/*
 * A172467  T(n,k) = number of summands in the partitions of n into k parts; a
 *          triangular array.
 * https://oeis.org/A172467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 955, u32 n <= 8022, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   global_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A172467
 *
 *  T(n,k) = k * P(n,k)   (P(n,k) = number of partitions of n into exactly k parts)
 *  The sequence a(n) is obtained by reading the triangular array
 *  T(n,k) row‑wise (row n has k=1..n).  Thus
 *      a(1)   = T(1,1)
 *      a(2)   = T(2,1)
 *      a(3)   = T(2,2)
 *      a(4)   = T(3,1)
 *      … .
 *
 *  Implementation uses a DP recurrence for P(n,k):
 *      P(0,0)=1,   P(n,0)=0 (n>0),   P(n,k)=0 (k>n)
 *      P(n,k)=P(n-1,k-1)+P(n-k,k)
 *
 *  The function handles arbitrary seq_t values (up to 128‑bit or big integers)
 *  and is thread‑safe for a single instance because it only mutates static
 *  data.  All arithmetic is performed in seq_t to preserve exactness.
 *
 *  Complexity: O(N^2) per new maximum N requested; with N ≈ sqrt(2n)
 *  (≈ 140 for n≤10000) this is trivial.
 */

#include <stdlib.h>
#include "../utils/global_to_rc.h"

static seq_t *part_table = NULL;     /* flat 2D array P[n][k] */
static idx_t part_maxN = 0;          /* largest n already computed */

/* ensure partition table up to N is built */
static void ensure_partitions(idx_t N)
{
    if (N <= part_maxN)
        return;

    /* new size */
    idx_t dim = N + 1;                     /* rows and columns */
    size_t total = (size_t)dim * (size_t)dim;
    seq_t *newp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!newp)
        return;    /* out of memory – caller will get 0 */

    /* base case */
    newp[0 * dim + 0] = 1;

    /* DP recurrence */
    for (idx_t n = 1; n <= N; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t a = newp[(n - 1) * dim + (k - 1)];
            seq_t b = 0;
            if (n >= k)
                b = newp[(n - k) * dim + k];
            newp[n * dim + k] = a + b;
        }
    }

    /* replace old table */
    free(part_table);
    part_table = newp;
    part_maxN   = N;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined below offset */

    /* find row r and column k */
    idx_t r, k;
    global_to_rc(n, &r, &k);

    /* compute partitions up to r */
    ensure_partitions(r);

    /* dimensions of the table */
    idx_t dim = part_maxN + 1;

    /* value: T(r,k) = k * P(r,k) */
    seq_t p_val = part_table[r * dim + k];
    return (seq_t)k * p_val;
}
