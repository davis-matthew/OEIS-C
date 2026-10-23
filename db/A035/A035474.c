/*
 * A035474  Coordination sequence for lattice D*_10 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{10} 2^k·C(10,k)·C(n-1,k-1) + 2^10·C(n+4,9), *
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 245, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is given explicitly */
    if (n == 0) return 1;

    /* Pre‑computed coefficients 2^k * C(10,k) for k = 0 .. 10 */
    static const seq_t factor[11] = {
        1,     /* 2^0 * C(10,0)  = 1    */
        20,    /* 2^1 * C(10,1)  = 20   */
        180,   /* 2^2 * C(10,2)  = 180  */
        960,   /* 2^3 * C(10,3)  = 960  */
        3360,  /* 2^4 * C(10,4)  = 3360 */
        8064,  /* 2^5 * C(10,5)  = 8064 */
        13440, /* 2^6 * C(10,6)  = 13440*/
        15360, /* 2^7 * C(10,7)  = 15360*/
        11520, /* 2^8 * C(10,8)  = 11520*/
        5120,  /* 2^9 * C(10,9)  = 5120 */
        1024   /* 2^10 * C(10,10)= 1024*/
    };

    seq_t result = 0;

    /* First summation: Σ_{k=1}^{10} factor[k] * C(n-1, k-1) */
    for (idx_t k = 1; k <= 10; ++k) {
        result += factor[k] * binomial(n - 1, k - 1);
    }

    /* Second term: factor[10] * C(n+4, 9)  (since 2^10 = factor[10]) */
    result += factor[10] * binomial(n + 4, 9);

    return result;
}
