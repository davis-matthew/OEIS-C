/*
 * A039899  Number of partitions satisfying 0 < cn(0,5) + cn(2,5) + cn(3,5).
 * https://oeis.org/A039899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039899 – Number of partitions of n for which at least one part is
   congruent to 0, 2, or 3 (mod 5).  The sequence can be obtained as
   a(n) = p(n) – q(n), where p(n) is the unrestricted partition function,
   and q(n) counts partitions that use only parts congruent to 1 or 4 (mod 5). */

/* --------------------------------------------------------------------- */
/* Requires:   seq_t and idx_t (unsigned integer types) are defined by
 *             the build system.  The code must compile under C11 and
 *             C++17.                                                    */
/* --------------------------------------------------------------------- */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Cache of unrestricted partition numbers up to the largest n
 *  requested so far.  The array is recomputed from scratch whenever a
 *  larger n is requested.  For n up to about 10 000 the O(n²) algorithm
 *  used below runs comfortably within 2 s.                               */
/* --------------------------------------------------------------------- */
static seq_t *part_arr = NULL;  /* p[0..max_part_n] */
static idx_t  max_part_n = 0;

/* Ensure that part_arr contains partitions p[0] … p[up_to] */
static void ensure_part(idx_t up_to)
{
    if (max_part_n >= up_to)
        return;                         /* already computed */

    idx_t newN   = up_to;
    size_t sz   = (size_t)(newN + 1);
    seq_t *new_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_arr)                      /* allocation failure */
        exit(EXIT_FAILURE);

    /* Initialise to zero, then set base case p[0] = 1 */
    for (idx_t i = 0; i <= newN; ++i)
        new_arr[i] = 0;
    new_arr[0] = 1;

    /* Classic dynamic‑programming partition count:
       for each part size j, add j to all sums that can contain it. */
    for (idx_t part = 1; part <= newN; ++part) {
        for (idx_t sum = part; sum <= newN; ++sum) {
            new_arr[sum] += new_arr[sum - part];
        }
    }

    /* Replace old cache */
    free(part_arr);
    part_arr   = new_arr;
    max_part_n = newN;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 0 by definition of the sequence */
        return 0;

    /* 1. Compute unrestricted partition number p(n) */
    ensure_part(n);
    seq_t part = part_arr[n];

    /* 2. Compute q(n): partitions whose parts are only 1 or 4 (mod 5) */
    size_t len = (size_t)(n + 1);
    seq_t *q = (seq_t *)malloc(len * sizeof(seq_t));
    if (!q)                     /* allocation failure */
        exit(EXIT_FAILURE);

    for (size_t i = 0; i <= n; ++i)
        q[i] = 0;
    q[0] = 1;                    /* empty partition */

    for (idx_t part = 1; part <= n; ++part) {
        if (part % 5 == 1 || part % 5 == 4) {
            for (idx_t s = part; s <= n; ++s) {
                q[s] += q[s - part];
            }
        }
    }

    seq_t ans = part - q[n];      /* q[n] ≤ part[n] always */
    free(q);
    return ans;
}
