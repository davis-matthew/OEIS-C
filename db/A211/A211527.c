/*
 * A211527  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three, four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 16*a(n-3) + 27*a(n-4) + 8*a(n-5) -
 *            35*a(n-6) + 10*a(n-7) + 10*a(n-8) - 4*a(n-9) Initial values:
 *            a(1)=8, a(2)=18, a(3)=42, a(4)=90, a(5)=192, a(6)=394, a(7)=806,
 *            a(8)=1618, a(9)=3244 (All other values are computed by the
 *            recurrence above.)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The build supplies typedefs for seq_t (the sequence element type) and
   idx_t (the index type).  No additional headers are required for this
   implementation. */
   
/* Maximum value of n expected in the test cases (~10000).  Increase if
   needed. */
#define MAX_N 10001

seq_t a(idx_t n)
{
    /* Static table holding the sequence values we've already computed.
       Index 0 corresponds to a(1). */
    static seq_t seq[MAX_N];
    static size_t computed = 0;            /* Number of terms currently stored. */
    static const seq_t init[9] = {
        8ULL, 18ULL, 42ULL, 90ULL,
        192ULL, 394ULL, 806ULL, 1618ULL, 3244ULL
    };

    /* Guard against out‑of‑range n.  Behaviour below the offset is
       undefined, but we must not crash. */
    if (n == 0 || (size_t)n >= MAX_N)
        return 0;

    /* If this is the first call or we haven't initialised the first
       nine terms yet, do so now. */
    if (computed == 0) {
        for (size_t i = 0; i < 9; ++i)
            seq[i] = init[i];
        computed = 9;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n <= computed)
        return seq[n-1];

    /* Compute the missing terms up to n using the recurrence relation. */
    for (size_t i = computed; i < (size_t)n; ++i) {
        /* i is the 0‑based index for a(i+1).  All indices used below are
           valid because i >= 9 when this loop starts (since computed >= 9). */
        seq_t pos = 5 * seq[i-1]          /* a(n-1)   */
                  + 27 * seq[i-4]         /* a(n-4)   */
                  + 8  * seq[i-5]         /* a(n-5)   */
                  + 10 * seq[i-7]         /* a(n-7)   */
                  + 10 * seq[i-8];        /* a(n-8)   */

        seq_t neg = 4 * seq[i-2]           /* a(n-2)   */
                  + 16 * seq[i-3]          /* a(n-3)   */
                  + 35 * seq[i-6]          /* a(n-6)   */
                  + 4  * seq[i-9];         /* a(n-9)   */

        seq[i] = pos - neg;                /* final value */
    }
    computed = (size_t)n;

    return seq[n-1];
}
