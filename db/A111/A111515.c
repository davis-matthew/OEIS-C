/*
 * A111515  Number of partitions of T where T=(7*n + 1) if n is even and
 *          T=((7*n + 1)/2) if n is odd.
 * https://oeis.org/A111515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(T), where T = 7*n+1 for even n, (7*n+1)/2 for odd n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(k) is the partition function of k.  The routine uses the
   recurrence based on generalized pentagonal numbers. */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *part = NULL;      /* array of partition numbers */
static idx_t  cap   = 0;        /* current capacity of 'part' */
static idx_t  maxT  = 0;        /* highest T computed so far */

/* Extend 'part' to hold values up to 'upto' inclusive.
   New entries are computed using the recurrence. */
static void precompute(idx_t upto)
{
    if (upto <= maxT)
        return;

    idx_t newCap = upto + 1;

    /* Allocate a larger array if needed. */
    if (part == NULL) {
        part = alloc_seq_array(newCap);
        /* part[0] = 1, the rest are zero (alloc_seq_array returns zeroed). */
        part[0] = 1;
    } else if (newCap > cap) {
        seq_t *newPart = alloc_seq_array(newCap);
        for (idx_t i = 0; i <= maxT; ++i)
            newPart[i] = part[i];
        /* remaining elements stay zero. */
        free_seq_array(part);
        part = newPart;
    }
    cap = newCap;

    /* Compute missing p(i) for i = maxT+1 .. upto. */
    for (idx_t i = maxT + 1; i <= upto; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)
                pos += part[i - pent1];
            else
                neg += part[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1)
                pos += part[i - pent2];
            else
                neg += part[i - pent2];
        }
        part[i] = pos - neg;   /* result is non‑negative */
    }
    maxT = upto;
}

seq_t a(idx_t n)
{
    /* Compute the target number T according to parity of n. */
    idx_t Tidx;
    if ((n & 1) == 0)
        Tidx = 7 * n + 1;
    else
        Tidx = (7 * n + 1) / 2;

    precompute(Tidx);
    return part[Tidx];
}
