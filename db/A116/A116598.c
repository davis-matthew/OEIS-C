/*
 * A116598  Triangle read by rows: T(n,k) is the number of partitions of n
 *          having exactly k parts equal to 1 (n>=0, 0<=k<=n).
 * https://oeis.org/A116598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle of counts of partitions of r
 *            having exactly k parts equal to 1 (T(r,k)). For a linear index
 *            N: let r be the unique integer with offset_row(r) <= N <
 *            offset_row(r+1) and let c = N - offset_row(r). Then a(N) =
 *            T(r,c), where - if c == r, T(r,r) = 1 - otherwise T(r,c) =...
 * Range:     u8 n <= 299, u16 n <= 1595, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 121 terms through n = 8976 (2026-11-03)
 * Helpers:   mem, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation builds partition numbers up to the
   row currently needed and evaluates T(r,c) using the known formula. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/triangular.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *part = NULL;     /* cache of partition numbers p[0..part_n] */
static idx_t part_n = 0;

/* Compute partition numbers p[0..n] (inclusive) and store in part.
   p[0] = 1;  for i = 1..n, for j = i..n: p[j] += p[j-i]. */
static void compute_partitions_upto(idx_t n)
{
    seq_t *newp = alloc_seq_array((size_t)(n + 1));
    if (!newp) abort();

    /* initialize to zero */
    for (idx_t i = 0; i <= n; ++i) newp[i] = 0;
    newp[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            newp[j] += newp[j - i];
        }
    }

    if (part) free_seq_array(part);
    part = newp;
    part_n = n;
}

seq_t a(idx_t N)
{
    /* Find row r so that offset_row(r) <= N < offset_row(r+1) */
    idx_t r = 0;
    while (triangular(r) <= N) ++r;
    --r;

    idx_t offset = triangular(r);
    idx_t col = N - offset;

    /* Ensure partitions up to r are known */
    if (r > part_n) compute_partitions_upto(r);

    /* Evaluate T(r,col) */
    if (col == r) {
        return (seq_t)1;
    } else {
        idx_t m = r - col;          /* m = r - k */
        seq_t val = part[m];
        seq_t sub = (m > 0) ? part[m - 1] : 0;
        return val - sub;
    }
}
