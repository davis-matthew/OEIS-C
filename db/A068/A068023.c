/*
 * A068023  Z(S_m; sigma[1](n), sigma[2](n),..., sigma[m](n)) where Z(S_m;
 *          x_1,x_2,...,x_m) is the cycle index of the symmetric group S_m and
 *          sigma[k](n) is the sum of k-th powers of divisors of n; m=6.
 * https://oeis.org/A068023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/720 * (σ1(n)^6 + 15*σ1(n)^4*σ2(n) + 40*σ1(n)^3*σ3(n) *
 *            + 45*σ1(n)^2*σ2(n)^2 + 90*σ1(n)^2*σ4(n) * +
 *            120*σ1(n)*σ2(n)*σ3(n) + 15*σ2(n)^3 * + 144*σ1(n)*σ5(n) +
 *            90*σ2(n)*σ4(n) * + 40*σ3(n)^2 + 120*σ6(n) ) * * Here σk(n) is
 *            the sum of the k-th powers of the divisors of n. * This function
 *            uses...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, guard anyway */

    /* compute sigma_k for k=1..6 */
    seq_t s1 = sigma_k(n, 1);
    seq_t s2 = sigma_k(n, 2);
    seq_t s3 = sigma_k(n, 3);
    seq_t s4 = sigma_k(n, 4);
    seq_t s5 = sigma_k(n, 5);
    seq_t s6 = sigma_k(n, 6);

    /* powers of s1 and s2 */
    seq_t s1p2 = s1 * s1;
    seq_t s1p3 = s1p2 * s1;
    seq_t s1p4 = s1p3 * s1;
    seq_t s1p5 = s1p4 * s1;
    seq_t s1p6 = s1p5 * s1;

    seq_t s2p2 = s2 * s2;
    seq_t s2p3 = s2p2 * s2;

    seq_t s3p2 = s3 * s3;

    /* assemble the sum */
    seq_t sum = s1p6
              + (seq_t)15 * s1p4 * s2
              + (seq_t)40 * s1p3 * s3
              + (seq_t)45 * s1p2 * s2p2
              + (seq_t)90 * s1p2 * s4
              + (seq_t)120 * s1 * s2 * s3
              + (seq_t)15 * s2p3
              + (seq_t)144 * s1 * s5
              + (seq_t)90 * s2 * s4
              + (seq_t)40 * s3p2
              + (seq_t)120 * s6;

    /* divide by 720 */
    return sum / (seq_t)720;
}
