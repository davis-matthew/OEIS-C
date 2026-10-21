/*
 * A224462  The hyper-Wiener index of Wang's helicene-based dendrimer defined
 *          pictorially as DNS_2[n] in the Shabani et al. reference.
 * https://oeis.org/A224462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula from OEIS A224462:
   a(n) = -61550 + 2^n*(30861*n^2 - 164298*n - 602830)
          + 4^n*(59049*n^2 - (677241*n)/2 + 664380).  */
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset starts at 1; return 0 for safety */
        return 0;

    /* signed intermediate for the (possibly negative) part A */
    int64_t partA_int;
    {
        int64_t nn = (int64_t)n;
        partA_int = 30861*nn*nn - 164298*nn - 602830;
    }

    /* compute 2^n and 4^n */
    const seq_t pow2n = pow2(n);
    const seq_t pow4n = pow4(n);

    /* absolute value of part A for multiplication */
    const seq_t partA_abs = (seq_t)(partA_int >= 0 ? partA_int : -partA_int);
    const seq_t termA = pow2n * partA_abs;          /* |2^n * partA| */

    /* part B calculation without fractional division:
       4^n * (59049*n^2 + 664380) - (4^n/2) * 677241*n
    */
    const seq_t n_seq = (seq_t)n;

    const seq_t termB1 = pow4n * (seq_t)59049 * n_seq * n_seq;
    const seq_t termB2 = pow4n * (seq_t)664380;
    const seq_t termB3 = (pow4n / (seq_t)2) * (seq_t)677241 * n_seq;

    const seq_t termB = termB1 + termB2 - termB3;

    /* assemble final result */
    seq_t result = termB;
    if (partA_int >= 0)
        result += termA;     /* partA is positive */
    else
        result -= termA;     /* partA is negative */

    result -= (seq_t)61550;   /* constant term */

    return result;
}
