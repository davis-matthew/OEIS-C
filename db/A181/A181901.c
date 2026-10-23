/*
 * A181901  a(n) = least positive integer m such that 2(s_k)^2 for k=1,...,n
 *          are pairwise distinct modulo m where s_k = Sum_{j=1..k}
 *          (-1)^(k-j)*p_j and p_j is the j-th prime.
 * https://oeis.org/A181901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181901:  a(n) = least positive m such that
   2*(∑_{j=1..k} (-1)^{k-j} p_j)^2  (k=1..n) are pairwise distinct mod m.
   According to Sun's conjecture (verified up to 3⋅10^5), a(n)=p_{n+1} for all
   n except n=1,2,4,9 where a(1)=1, a(2)=4, a(4)=9, a(9)=25. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 4) return (seq_t)9;
    if (n == 9) return (seq_t)25;
    return (seq_t)get_prime(n + 1);
}
