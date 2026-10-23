/*
 * A177174  Quotients of A024167(4n+3) and 6n+5.
 * https://oeis.org/A177174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A024167(4n+3)/(6n+5) (OEIS A177174)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A024167(m) = m! * (1 - 1/2 + 1/3 - … + (-1)^(m+1)/m)  */
/* The value a(n) is an integer. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 4n+3 , divisor = 6n+5 */
    seq_t m      = (seq_t)n * 4 + 3;
    seq_t div    = (seq_t)n * 6 + 5;
    idx_t  mi    = (idx_t)m;          /* m fits in idx_t for the ranges used */

    seq_t A  = (seq_t)1;   /* A(1) = 1 */
    seq_t fact = (seq_t)1; /* (i-1)! for the first iteration   */

    for (idx_t i = 2; i <= mi; ++i) {
        seq_t temp = (seq_t)i * A;   /* i * A_{i-1} */
        if (i & 1)                    /* i odd -> plus sign   */
            A = temp + fact;
        else                          /* i even -> minus sign */
            A = temp - fact;
        fact *= (seq_t)i;              /* update factorial to i!   */
    }

    /* A is now A(4n+3).  Compute a(n). */
    return A / div;   /* exact division, result is integer   */
}
