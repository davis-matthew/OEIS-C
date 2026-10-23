/*
 * A199923  a(n) = Sum_{k=0..3^(n-1)} gcd(k,3^(n-1)) for n > 0 and a(0) = 1.
 * https://oeis.org/A199923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=0), otherwise a(n) = 2*(n+2)*3^(n-2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* handle the special cases first */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* for n >= 2 use the reduced power 3^(n-2) to avoid overflow */
    seq_t pow3_n_2 = pow3(n - 2);            /* 3^(n-2) */
    seq_t mult = (seq_t)2 * ((seq_t)n + 2);  /* 2*(n+2) */
    return mult * pow3_n_2;
}
