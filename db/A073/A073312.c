/*
 * A073312  Number of nonsquarefree numbers in the reduced residue system of
 *          n.
 * https://oeis.org/A073312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonsquarefree residues in the reduced residue
 *            system of n
 * Range:     u8 n <= 657, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Euclidean algorithm for unsigned values */
static inline unsigned long long _gcd_uint64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t nonsquare_count = 0;
    for (idx_t r = 1; r <= n; ++r) {
        if (_gcd_uint64((unsigned long long)r, (unsigned long long)n) == 1) {
            if (!is_squarefree((seq_t)r)) {
                ++nonsquare_count;
            }
        }
    }
    return nonsquare_count;
}
