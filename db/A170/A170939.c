/*
 * A170939  a(n) = 4^n - 2^n + 2.
 * https://oeis.org/A170939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^n + 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute 4^n and 2^n using the provided pow utility functions.
       The subtraction and addition are performed using the native
       operators; this works for all supported seq_t types (built‐in
       unsigned types as well as arbitrary‑precision integer classes). */
    seq_t pow4_n = pow4(n);   /* 4^n */
    seq_t pow2_n = pow2(n);   /* 2^n */
    seq_t result = pow4_n - pow2_n + (seq_t)2;
    return result;
}
