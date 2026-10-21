/*
 * A225367  Number of palindromes of length n in base 3 (A118594).
 * https://oeis.org/A225367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*3^floor((n-1)/2) + [n==1]
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-2) for n>3 (not used directly) */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the case n==0 gracefully, though not part of the sequence */
    if (n == 0)
        return 0;

    idx_t k = (n - 1) / 2;      /* floor((n-1)/2) */
    seq_t res = pow3(k);        /* 3^k */
    res *= 2;                   /* 2 * 3^k */

    if (n == 1)
        res += 1;               /* add indicator for n==1 */

    return res;
}
