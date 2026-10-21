/*
 * A206558  Number of 8's in the last section of the set of partitions of n.
 * https://oeis.org/A206558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-8) + p(n-8) - p(n-9) (n>8), with initial values
 *            a(1)=…=a(7)=0, a(8)=1. Here p(k) denotes the partition number
 *            p(k). The partition numbers are computed on demand using Euler’s
 *            pentagonal‑number theorem
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the usual headers */
#include <stdlib.h>

static seq_t *part   = NULL;          /* partition numbers, part[0]==1 */
static seq_t *aseq   = NULL;          /* sequence values a(n) */
static size_t  cap   = 0;             /* allocated array size   */
static idx_t   last_n= 0;             /* highest a(n) computed  */
static idx_t   last_p= 0;             /* highest part index computed */

static void ensure_capacity(size_t need)
{
    if (cap <= need) {
        size_t new_cap = cap? cap<<1 : 16;
        while (new_cap <= need) new_cap <<= 1;
        part = (seq_t *)realloc(part,  new_cap * sizeof(seq_t));
        aseq = (seq_t *)realloc(aseq, new_cap * sizeof(seq_t));
        /* initialise new slots to zero for safety */
        for (size_t i = cap; i < new_cap; ++i) part[i] = 0, aseq[i] = 0;
        cap = new_cap;
    }
}

/* Compute partition numbers up to target (inclusive) using Euler’s recurrence.
   After the call last_p == target. */
static void compute_partitions(idx_t target)
{
    if (part[0] == 0) part[0] = 1;

    for (idx_t i = last_p + 1; i <= target; ++i) {
        seq_t sum = 0;
        for (int k = 1;; ++k) {
            idx_t pent1 = (idx_t)k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            sum += (k & 1) ? part[i - pent1] : -part[i - pent1];

            idx_t pent2 = (idx_t)k * (3 * k + 1) / 2;
            if (pent2 <= i)
                sum += (k & 1) ? part[i - pent2] : -part[i - pent2];
        }
        part[i] = sum;
    }
    last_p = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for n<1 */

    /* Allocate storage for all indices up to n. */
    ensure_capacity(n);

    /* Ensure partition numbers are ready for indices up to n-1. */
    if (n > 1 && last_p < (n - 1))
        compute_partitions(n - 1);

    /* Initialise the first eight terms if not yet done. */
    if (last_n < 8) {
        for (idx_t i = 1; i <= 8; ++i) {
            aseq[i] = (i == 8) ? 1 : 0;
        }
        last_n = 8;
    }

    /* Compute a(k) for k = last_n+1 .. n. */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* For k <= 8 the values are already set. */
        aseq[k] = aseq[k - 8] + part[k - 8] - part[k - 9];
    }
    last_n = n;
    return aseq[n];
}
