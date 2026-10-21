/*
 * A204648  Number of (n+1) X 6 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 116, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A204648: Number of (n+1)×6 binary arrays with non‑decreasing 
 *  pair sums.
 *
 *  Recurrence (for n > 11):
 *      a(n) = 6*a(n-1) - 14*a(n-2) + 14*a(n-3) - 14*a(n-5)
 *             + 14*a(n-6) - 6*a(n-7) + a(n-8)
 *
 *  The implementation evaluates the recurrence iteratively using only
 *  addition, multiplication by small constants, and one subtraction
 *  per step.  The final subtraction is safe because the positive
 *  terms are guaranteed to exceed the negative terms for all n ≥ 12.
 *
 *  The function recomputes the value from scratch for each call; this
 *  yields O(n) time per invocation, well below the 2 s limit for n
 *  up to 10 000.
 */
#include <stddef.h>   /* size_t */

/* The build supplies these typedefs:
 *   typedef <unsigned integer type> idx_t;   // index type
 *   typedef <unsigned integer or big‑int> seq_t; // value type
 */

/* Initial values a(1)…a(11) */
static const seq_t base[12] = {
    0,          /* unused, 1‑based indexing */
    80, 137, 284, 571, 1076, 1918, 3261, 5329, 8408, 12867, 19162
};

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;           /* undefined but harmless */

    if (n <= 11)
        return base[(idx_t)n];

    /* Circular buffer holding a[k-8], ..., a[k-1] for the current k. */
    seq_t buf[8];

    /* Initialise buf with a[4]..a[11] so that the first iteration
       (k = 12) has the correct preceding values. */
    buf[0] = base[4];   /* a[4] = a(k-8) when k=12 */
    buf[1] = base[5];   /* a[5] = a(k-7) */
    buf[2] = base[6];   /* a[6] = a(k-6) */
    buf[3] = base[7];   /* a[7] = a(k-5) */
    buf[4] = base[8];   /* a[8] = a(k-4) */
    buf[5] = base[9];   /* a[9] = a(k-3) */
    buf[6] = base[10];  /* a[10] = a(k-2) */
    buf[7] = base[11];  /* a[11] = a(k-1) */

    /* Iterate from k = 12 up to n */
    for (idx_t k = 12; k <= n; ++k)
    {
        /* Positive part: 6*a(k-1) + 14*a(k-3) + 14*a(k-6) + a(k-8) */
        seq_t big = (seq_t)6 * buf[7] + (seq_t)14 * buf[5]
                    + (seq_t)14 * buf[2] + buf[0];

        /* Negative part: 14*a(k-2) + 14*a(k-5) + 6*a(k-7) */
        seq_t small = (seq_t)14 * buf[6] + (seq_t)14 * buf[3]
                      + (seq_t)6 * buf[1];

        /* New term = positive part – negative part */
        seq_t newval = big - small;

        /* Shift buffer left: remove a(k-8), add newval as a(k-1) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = newval;
    }

    /* After the loop, buf[7] holds a[n] */
    return buf[7];
}
