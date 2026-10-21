/*
 * A224464  The hyper-Wiener index of the tetrathiafulvalene dendrimer defined
 *          pictorially as D[n] in the Shabani reference.
 * https://oeis.org/A224464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224464 – Hyper‑Wiener index of the tetrathiafulvalene dendrimer
   Formula: a(n) = 1489593 + 2^n*(-190960*n^2 +743480*n - 13082162)
                + 4^n*(3013696*n^2 - 7398832*n + 11662728)
   (The formula uses n=0 for the first term; here a(idx_t n) expects
   the OEIS offset, so n=1 corresponds to n=0 in the formula.) */
#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp);  2^exp */
#include "../utils/pow4.h"   /* seq_t pow4(idx_t exp);  4^exp */

seq_t a(idx_t n)
{
    /* shift to 0‑based index used in the closed form */
    idx_t k = n - 1;                 /* k can be 0,1,2,… */

    /* compute powers 2^k and 4^k (seq_t type) */
    seq_t pow2k = pow2(k);           /* 2^k */
    seq_t pow4k = pow4(k);           /* 4^k */

    /* signed values of the polynomial coefficients */
    long long kk   = (long long)k;
    long long ksq   = kk * kk;               /* n^2 */

    long long p1 = -190960LL * ksq + 743480LL * kk - 13082162LL;       /* coeff for 2^k (can be negative) */
    long long p2 = 3013696LL  * ksq - 7398832LL  * kk + 11662728LL;     /* coeff for 4^k (always non‑negative) */

    /* start with the constant term */
    seq_t result = (seq_t)1489593ULL;

    /* add the 4^k term (coefficient is non‑negative) */
    result += pow4k * (seq_t)p2;

    /* add or subtract the 2^k term depending on the sign of p1 */
    if (p1 >= 0)
        result += pow2k * (seq_t)p1;
    else
        result -= pow2k * (seq_t)(-p1);

    return result;
}
