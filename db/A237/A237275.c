/*
 * A237275  Smallest k divisible by the n-th power of its last decimal digit >
 *          1.
 * https://oeis.org/A237275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3*2^n if n mod 4 = 2; 2^(n+2-((n+1) mod 4)) otherwise
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t mod = n % 4;
    seq_t res;
    if (mod == 2) {
        res = pow2(n);     /* 2^n */
        res = res * 3;     /* 3*2^n */
    } else {
        /* exponent = n + 2 - ((n + 1) mod 4) */
        idx_t exp = n + 2 - ((n + 1) % 4);
        res = pow2(exp);   /* 2^exp */
    }
    return res;
}
