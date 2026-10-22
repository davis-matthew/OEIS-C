/*
 * A000891  a(n) = (2*n)!*(2*n+1)! / (n! * (n+1)!)^2.
 * https://oeis.org/A000891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000108(n)*A001700(n) = (binomial(2*n,n)/(n+1)) *
 *            binomial(2*n+1,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   a891_binom, a891_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a891_binom.h"
#include "../utils/a891_gcd.h"

seq_t a(idx_t n) {
    seq_t c = a891_binom(2 * n, n);
    seq_t den = (seq_t)n + 1;
    seq_t cat = c / den;
    seq_t b = a891_binom(2 * n + 1, n);
    return cat * b;
}
