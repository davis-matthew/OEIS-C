/*
 * A008047  Coordination sequence T1 for Zeolite Code BIK.
 * https://oeis.org/A008047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{14} q_k * a(n-k) - p[n] * where q =
 *            {-1,0,-2,2,0,4,1,0,-1,-4,0,-2,2,0,1} * p =
 *            {-1,-4,-14,-29,-59,-89,-120,-132,-128,-101,-67,-31,-12,0,1,2} *
 *            (For n > 15, p[n] = 0.)
 * Range:     u8 n <= 9, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int q[15] = {
    -1, /* q0   */
     0, /* q1   */
    -2, /* q2   */
     2, /* q3   */
     0, /* q4   */
     4, /* q5   */
     1, /* q6   */
     0, /* q7   */
    -1, /* q8   */
    -4, /* q9   */
     0, /* q10  */
    -2, /* q11  */
     2, /* q12  */
     0, /* q13  */
     1  /* q14  */
};

static const int p[16] = {
    -1,  /* p0  */
    -4,  /* p1  */
   -14,  /* p2  */
   -29,  /* p3  */
   -59,  /* p4  */
   -89,  /* p5  */
  -120,  /* p6  */
  -132,  /* p7  */
  -128,  /* p8  */
  -101,  /* p9  */
   -67,  /* p10 */
   -31,  /* p11 */
   -12,  /* p12 */
     0,  /* p13 */
     1,  /* p14 */
     2   /* p15 */
};

seq_t a(idx_t n)
{
    static idx_t     len   = 0;              /* number of elements already computed   */
    static idx_t     cap   = 16;             /* current capacity in elements           */
    static seq_t    *data  = NULL;           /* array of computed terms                */

    /* Allocate if first call. */
    if (!data) {
        data = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Extend array if necessary. */
    while (len <= n) {
        idx_t i = len;          /* index of element to compute          */
        long long sum = 0;      /* signed accumulator (small enough)   */

        /* q[0] = -1, so we can compute a[i] as sum of q[k]*a[i-k] for k=1..14
           plus the subtraction of p[i] (if existing).       */
        for (int k = 1; k < 15; ++k) {
            if (i >= (idx_t)k)
                sum += (long long)q[k] * (long long)data[i - k];
        }
        if (i <= 15)
            sum -= (long long)p[i];      /* subtract p[i] (const)                */

        data[i] = (seq_t)sum;            /* cast to the actual type              */
        ++len;

        /* Grow the array if necessary. */
        if (len == cap) {
            cap <<= 1;                   /* double the capacity                  */
            data = (seq_t *)realloc(data, cap * sizeof(seq_t));
        }
    }

    return data[n];
}
