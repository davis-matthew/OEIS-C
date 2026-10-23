/*
 * A024155  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          that are right triangles.
 * https://oeis.org/A024155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer-sided right triangles with integer
 *            sides a<b<c and perimeter a+b+c = n. The count is obtained by
 *            counting all representations of n as k·2·m·(m+t) where m>t>0, m
 *            and t coprime, and m−t is odd. Each such representation
 *            corresponds to one right triangle
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Cache for counts up to the largest n computed so far.  The array
   indices correspond directly to the perimeter value. */
static seq_t *perim_count = NULL;   /* cache of triangle counts */
static idx_t  cache_size  = 0;      /* number of indices currently stored */

/* -----------------------------------------------------------------------
   Compute all counts up to and including N (inclusive).  The array is
   reallocated to size N+1.  Each primitive Pythagorean triple is
   generated using Euclid's formula: 
         primitive perimeter  P0 = 2·m·(m+t)     (m>t>0, m−t odd, gcd(m,t)=1)
   Every multiple k·P0 ≤ N gives a distinct right triangle with that
   perimeter.
   ----------------------------------------------------------------------- */
static void compute_counts(idx_t N)
{
    size_t sz = (size_t)(N + 1);                 /* array length */
    seq_t *new_cnt = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_cnt)
        return;                                 /* allocation failed: keep old cache */

    /* Initialise counts to zero. */
    memset(new_cnt, 0, sz * sizeof(seq_t));

    /* Generate primitive triples. */
    for (idx_t m = 2; 2 * m * (m + 1) <= N; ++m) {          /* limit m by lowest t=1 */
        for (idx_t t = 1; t < m; ++t) {
            /* Only consider pairs of opposite parity. */
            if (((m - t) & 1) == 0)
                continue;

            /* gcd for idx_t values via the helper from utils/gcd.h.  The operands
               are small (≤ sqrt(N/2)), so fitting in seq_t is safe. */
            if (gcd((seq_t)m, (seq_t)t) != 1)
                continue;

            idx_t base = 2 * m * (m + t);   /* primitive perimeter */
            if (base > N)
                continue;

            /* Mark all multiples of the primitive perimeter. */
            for (idx_t k = base; k <= N; k += base)
                new_cnt[k] += (seq_t)1;
        }
    }

    /* Replace the old cache with the freshly computed one. */
    free(perim_count);
    perim_count = new_cnt;
    cache_size  = sz;
}

/* -----------------------------------------------------------------------
   Public interface required by the problem statement.
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* outside the offset; return 0 */
        return (seq_t)0;

    /* Expanding cache if necessary. */
    if (perim_count == NULL || n >= cache_size)
        compute_counts(n);

    return perim_count[n];
}
