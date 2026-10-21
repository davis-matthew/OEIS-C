/*
 * A239969  Least positive k such that triangular(n) + triangular(n+k) is a
 *          triangular number (A000217), or -1 if no such k exists.
 * https://oeis.org/A239969
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A239969:  a(n) = least positive k such that 
    triangular(n) + triangular(n+k) is triangular,
    or -1 if no such k exists (for n < 3).       
    Formula: a(n) = least k>0 with 4k² + 8(k+1)n + 8n² + 4k + 1 = m² for some integer m.
*/

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* Integer square root (floor) for a generic unsigned type seq_t.
   Works for seq_t up to 128 bits or arbitrary‑precision, using only
   integer operations allowed in the constraints. */
static seq_t
isqrt_seq_t(seq_t x)
{
    if (x == 0 || x == 1)
        return x;

    seq_t low  = 0;
    seq_t high = x;                      /* high is guaranteed to be >= sqrt(x) */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;   /* upper mid */
        /* avoid overflow: mid*mid <= x  ⇔  mid <= x/mid  (mid > 0) */
        if (mid <= x / mid)
            low = mid;               /* mid is too small or just right */
        else
            high = mid - 1;          /* mid is too large */
    }
    return low;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* OEIS offset: first term is a(3). For n < 3 we return -1. */
    if (n < 3)
        return (seq_t)-1;

    seq_t nn = (seq_t)n;                 /* cast index to seq_t for arithmetic */

    /* Precompute 8*n^2 part of the discriminant. */
    seq_t base = (seq_t)8 * nn * nn;     /* fits in seq_t for all n in the limits */

    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;

        /* Discriminant of the quadratic equation m² = 4k² + 8(k+1)n + 8n² + 4k + 1. */
        seq_t D = (seq_t)4 * kk * kk
                + (seq_t)8 * (kk + 1) * nn
                + base
                + (seq_t)4 * kk
                + (seq_t)1;

        seq_t s = isqrt_seq_t(D);
        if (s * s == D)          /* perfect square found */
            return kk;           /* k is the answer */
        /* No explicit upper bound; the search will terminate once a perfect
           square is found, which always happens for n >= 3 in practice.  */
    }
    /* Unreachable but satisfies compiler. */
    return (seq_t)-1;
}
