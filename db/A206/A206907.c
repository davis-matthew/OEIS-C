/*
 * A206907  n+[nr/s]+[nt/s], where []=floor, r=1/3, s=sqrt(3), t=1/s.
 * https://oeis.org/A206907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   less_A_B, less_A_C, less_B_C
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = rank of n/√3 in the joint ranking of the sequences
   {3k}, {k/√3}, {k√3}. The algorithm merges the three sequences using
   only integer arithmetic, avoiding floating point. It returns the
   rank of the n-th element of the middle sequence. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/less_A_B.h"
#include "../utils/less_A_C.h"
#include "../utils/less_B_C.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; return 0 for n==0 */
    idx_t target = n;              /* we want the rank of the target B */
    idx_t iA = 1, iB = 1, iC = 1;  /* current indices of the three sequences */
    idx_t rank = 0;                /* number of elements processed so far */

    while (1) {
        bool a_lt_b = less_A_B(iA, iB);
        bool a_lt_c = less_A_C(iA, iC);
        bool b_lt_c = less_B_C(iB, iC);

        if (a_lt_b && a_lt_c) {           /* 3*iA is smallest */
            iA++;
            rank++;
        } else if (!a_lt_b && b_lt_c) {   /* iB/√3 is smallest */
            if (iB == target) {
                return (seq_t)(rank + 1); /* rank of the target */
            }
            iB++;
            rank++;
        } else {                          /* iC*√3 is smallest (ties: C before B) */
            iC++;
            rank++;
        }
    }
}
