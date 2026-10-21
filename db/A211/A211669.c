/*
 * A211669  Number of iterations f(f(f(...(n)...))) such that the result is <
 *          2, where f(x) = cube root of x.
 * https://oeis.org/A211669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n < 2^(3^k)
 * Range:     u8 n <= 7, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, the number of cube‑root iterations needed to bring n below 2) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, …  */
    if (n <= 1)
        return 0;

    idx_t exp = 1;           /* exp = 3^k, starting with k = 0 */
    unsigned k   = 0;        /* k is the current candidate answer          */

    for (;; ++k) {
        seq_t thresh = pow2(exp);    /* threshold = 2^(3^k)                   */
        if ((seq_t)n < thresh)       /* n is already smaller than the threshold */
            return (seq_t)k;         /* k is the required number of iterations */
        exp *= 3;    /* advance to the next exponent 3^(k+1) */
    }
}
