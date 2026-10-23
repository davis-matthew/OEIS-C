/*
 * A152235  Largest squarefree number dividing the number of divisors n.
 * https://oeis.org/A152235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = rad(tau(n)) where tau(n) is the number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 ; for n = 0 the function is not defined in the OEIS sequence */
    idx_t tau = divisor_count_u64(n);  /* τ(n) */
    return rad(tau);                    /* rad(τ(n)) */
}
