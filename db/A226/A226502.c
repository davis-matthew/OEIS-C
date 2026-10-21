/*
 * A226502  Let P(k) denote the k-th prime (P(1)=2, P(2)=3 ...); a(n) =
 *          P(n+1)P(n+3) - P(n)P(n+2).
 * https://oeis.org/A226502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = P(n+1)*P(n+3) - P(n)*P(n+2)
 * Range:     u8 n <= 8, u16 n <= 292, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n should be >=1 */
    if (n == 0) return 0;

    unsigned long long p0 = get_prime(n);
    unsigned long long p1 = get_prime(n + 1);
    unsigned long long p2 = get_prime(n + 2);
    unsigned long long p3 = get_prime(n + 3);

    unsigned long long res = p1 * p3 - p0 * p2;  /* 64‑bit arithmetic */
    return (seq_t)res;
}
