/*
 * A143228  Triangle read by rows, T(n,k) = p(n) * p(k), where p(n) = the
 *          number of partitions of n, for 0 <= k <= n.
 * https://oeis.org/A143228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 168, u32 n <= 1032, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the linearized triangular array T(r,k)=p(r)*p(k) (partition numbers).
   For 0≤k≤r, the row–r in the triangle contains
      T(r,0),T(r,1),…,T(r,r).
   The sequence a(n) lists the triangle elements row by row: a(0)=T(0,0),
   a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …          */

#include <stdlib.h>

/* cache of partition numbers p(0)…p(N) */
static seq_t *partitions = NULL;   /* array p[0..N] */
static idx_t  part_cap   = 0;      /* size currently allocated (=N+1) */

/* Compute partition numbers up to max_n inclusive.
   The algorithm uses the standard coin–change DP:
      p[0] = 1;  for coin = 1..max_n
                    for sum = coin..max_n
                        p[sum] += p[sum-coin];
   This is O(max_n²) but is fine for max_n ≈ 10 000. */
static void ensure_partitions(idx_t max_n)
{
    if (partitions == NULL || max_n >= part_cap-1) {
        /* Allocate or reallocate */
        size_t sz = (size_t)(max_n + 1);
        seq_t *p = (seq_t *)realloc(partitions, sz * sizeof(seq_t));
        if (!p) abort();       /* allocation failure */
        partitions = p;

        /* Initialize array */
        for (idx_t i = 0; i <= max_n; ++i) partitions[i] = 0;
        partitions[0] = (seq_t)1;

        /* DP to compute partitions */
        for (idx_t coin = 1; coin <= max_n; ++coin) {
            for (idx_t sum = coin; sum <= max_n; ++sum) {
                partitions[sum] += partitions[sum - coin];
            }
        }
        part_cap = sz;
    }
}

/* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r * (r + 1) / 2) <= n) ++r;
    return r - 1;
}

/* OEIS sequence a(n). */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);            /* row index */
    idx_t offset = r * (r + 1) / 2;   /* starting index of row r */
    idx_t k = n - offset;             /* column index within the row */

    idx_t need = r > k ? r : k;
    ensure_partitions(need);          /* make sure partition numbers are ready */

    return partitions[r] * partitions[k];
}
