/*
 * A140434  Number of new visible points created at each step in an n X n
 *          grid.
 * https://oeis.org/A140434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=2*phi(n) for n>=2, where phi is Euler’s totient
 *            function
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* phi is defined as a static helper in phi.h */
    seq_t p = phi((idx_t)n);
    return p * (seq_t)2;
}
