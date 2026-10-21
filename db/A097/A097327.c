/*
 * A097327  Least positive integer m such that m*n has greater decimal digit
 *          length than n.
 * https://oeis.org/A097327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ceil(10^digits(n) / n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   num_dec_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digits(n) = floor(log10(n)) + 1, number of decimal digits of n. */

#include <stdint.h>
#include "../utils/num_dec_digits.h"

/* Replace idx_t and seq_t with the types defined by the build.
   This code uses only integer operations and a 128‑bit intermediate type
   (__uint128_t) which is supported by GCC/Clang. */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1; n>0 in practice */
    int d = num_dec_digits(n);             /* 1 <= d <= 20 for 64‑bit n */
    __uint128_t t = 1;                     /* t = 10^d, needs 128 bits */
    for (int i = 0; i < d; ++i) t *= 10;
    __uint128_t m = (t + n - 1) / n;       /* ceil division */
    return (seq_t)m;                       /* fits in seq_t (≤10) */
}
