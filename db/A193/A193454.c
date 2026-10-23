/*
 * A193454  Sum of odd divisors of phi(n).
 * https://oeis.org/A193454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all odd divisors of φ(n)
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The math helper is supplied by the build environment. */
#include <stdint.h>
#include "../utils/calc_phi.h"

/* Compute the sum of odd divisors of Euler's totient φ(n). */
seq_t a(idx_t n)
{
    /* Compute φ(n). */
    seq_t phi = calc_phi((seq_t)n);
    uint64_t phi_u = (uint64_t)phi;          /* safe for all n ≥ 1 ≤ 10 000 */

    seq_t sum = 0;
    /* Enumerate all divisors by checking i up to sqrt(phi). */
    for (uint64_t i = 1; i <= phi_u / i; ++i)
    {
        if (phi_u % i == 0)
        {
            if ((i & 1) != 0)                /* i is odd */
                sum += (seq_t)i;

            uint64_t j = phi_u / i;
            if (j != i && (j & 1) != 0)     /* j is the complementary divisor, odd */
                sum += (seq_t)j;
        }
    }
    return sum;
}
