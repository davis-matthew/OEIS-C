/*
 * A126981  Largest perfect square less than 2*10^n.
 * https://oeis.org/A126981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(2*10^n))^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Compute largest perfect square less than 2*10^n */
seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);           /* 10^n */
    seq_t limit  = pow10n * (seq_t)2;  /* 2*10^n */
    seq_t root   = floor_sqrt(limit);  /* floor(sqrt(limit)) */
    return root * root;                /* square of the root */
}
