/*
 * A090739  Exponent of 2 in 9^n - 1.
 * https://oeis.org/A090739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v2(n) + 3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  v2(n) is the exponent of 2 in the factorisation of n,
    i.e. the number of trailing zero bits in n. */

#include <stdint.h>

/* External linkage: no static keyword */
seq_t
a(idx_t n)
{
    seq_t res = 3;               /* base exponent from factor 8 = 2^3 */
    /* Count trailing zeros of n (its 2-adic valuation). */
    /* n is guaranteed non‑zero for the valid offset (n >= 1). */
    while ((n & 1) == 0) {
        n >>= 1;
        res++;
    }
    return res;
}
