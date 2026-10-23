/*
 * A194952  Number of Hamiltonian cycles in C_3 X C_n.
 * https://oeis.org/A194952
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 3/4*n*2^n + (2^n-(-2)^n)/2 + (-1)^n - 4
 *            Simplification: if n is even: a(n) = 3^n + (3*n*2^n)/4 – 3 if n
 *            is odd : a(n) = 3^n + (3*n*2^n)/4 + 2^n – 5 (indices start at n
 *            = 3 ; values for n < 3 are returned as 0)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                 /* behaviour below offset is unspecified */

    seq_t pow2_n = pow2(n);        /* 2^n   */
    seq_t pow3_n = pow3(n);        /* 3^n   */

    /* term = (3 * n * 2^n) / 4  -- integer division is exact for n>=2 */
    seq_t term = ((seq_t)3 * (seq_t)n * pow2_n) / 4;

    seq_t res = pow3_n + term;    /* common part of all n */

    if (n & 1) {                  /* odd n : +2^n – 5 */
        res += pow2_n;
        res -= (seq_t)5;
    } else {                      /* even n : – 3 */
        res -= (seq_t)3;
    }

    return res;
}
