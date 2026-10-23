/*
 * A185102  a(n) is the recursion depth of repeatedly factoring n and then the
 *          exponents in its prime product factorization, until 1 is reached.
 * https://oeis.org/A185102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=1+max{a(k_i)} where k_i are the exponents
 *            in the prime‑factorisation of n (the recurrence from the OEIS
 *            entry)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   depth_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/depth_calc.h"

/* Public entry point. */
seq_t a(idx_t n)
{
    return depth_calc(n);
}
