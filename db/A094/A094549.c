/*
 * A094549  Smallest m > n, coprime to n and having as many prime factors as
 *          n; a(1)=1.
 * https://oeis.org/A094549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > n such that GCD(m,n)=1 and (number of prime
 *            factors of m, counted with multiplicity) equals that of n.
 *            a(1)=1
 * Range:     u8 n <= 23, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"     /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/bigomega.h" /* seq_t bigomega(idx_t n); */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t omega_n = bigomega(n);
    idx_t m = n + 1;

    for (;; ++m) {
        if (gcd((seq_t)m, (seq_t)n) == 1 && bigomega(m) == omega_n)
            return (seq_t)m;
    }
}
