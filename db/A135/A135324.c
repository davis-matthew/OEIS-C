/*
 * A135324  a(n) = Sum_{k=1..phi(n)} k*t(k), where t(k) is the k-th positive
 *          integer which is coprime to n and phi(n) is the number of positive
 *          integers which are <= n and are coprime to n.
 * https://oeis.org/A135324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..phi(n)} k * t(k), where t(k) is the k-th
 *            positive * integer coprime to n (and <= n). t(k) are the coprime
 *            numbers in increasing * order
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 2346, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation iterates through 1..n, tests gcd(i,n)==1 and sums
 * cnt*i, where cnt is the number of coprimes seen so far. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cnt = 0;
    seq_t sn = (seq_t)n;           /* convert n once for gcd arguments */
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd((seq_t)i, sn) == 1) {
            ++cnt;
            sum += (seq_t)cnt * (seq_t)i;
        }
    }
    return sum;
}
