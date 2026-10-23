/*
 * A126261  a(n) is the numerator of the sum of the reciprocals of the
 *          positive integers k, k<=n, where every positive integer <= k and
 *          coprime to k is also coprime to n.
 * https://oeis.org/A126261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∑_{1 ≤ k ≤ n, Q(n,k) | k} 1/k, where
 *            Q(n,k)=∏_{p | n, p ≤ k} p (product of primes dividing n that are
 *            ≤ k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Determine the distinct prime factors of n (n ≤ 10 000 in normal tests). */
    uint64_t primes[64];
    size_t pcnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[pcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[pcnt++] = temp;

    /* Sum the reciprocals as a reduced fraction num/den. */
    seq_t num = 0;
    seq_t den = 1;
    size_t pi = 0;         /* index into primes array */
    seq_t cur_prod = 1;    /* product of primes dividing n that are ≤ current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update cur_prod when we reach a prime factor of n. */
        while (pi < pcnt && primes[pi] == k) {
            cur_prod *= (seq_t)primes[pi];
            ++pi;
        }

        /* k qualifies iff cur_prod divides k. */
        if ((seq_t)k % cur_prod == 0) {
            seq_t new_num = num * (seq_t)k + den;
            seq_t new_den = den * (seq_t)k;
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }
    }

    return num;
}
