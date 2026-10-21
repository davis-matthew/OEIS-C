/*
 * A053570  Sum of totient functions over arguments running through reduced
 *          residue system of n.
 * https://oeis.org/A053570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 ≤ k < n, gcd(k,n)=1} φ(k); a(1) = 1
 * Range:     u8 n <= 30, u16 n <= 466, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"

/* Euclidean algorithm on idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)         /* special case */
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k < n; ++k) {
        if (gcd_idx(k, n) == (idx_t)1) {
            sum += calc_phi((seq_t)k);
        }
    }
    return sum;
}
