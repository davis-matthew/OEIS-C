/*
 * A079977  Fibonacci numbers interspersed with zeros.
 * https://oeis.org/A079977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(k+1) if n = 2k, a(n) = 0 otherwise. Here
 *            A000045(k+1) is the (k+1)-th Fibonacci number with F(1)=1,
 *            F(2)=1. This implementation uses a fast‑doubling algorithm to
 *            compute F(k+1) for k = n/2
 * Range:     u8 n <= 25, u16 n <= 47, u32 n <= 93, u64 n <= 185,
 *            u128 n <= 371, big any n
 * Helpers:   fib_k_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_k_plus_one.h"

/* The sequence function a(n) for OEIS A079977.  */
seq_t a(idx_t n)
{
    if ((n & 1u) != 0)
        return (seq_t)0;               /* odd index → zero */

    idx_t k = n >> 1;                 /* n = 2k */
    return fib_k_plus_one(k);
}
