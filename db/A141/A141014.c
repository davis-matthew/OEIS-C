/*
 * A141014  E.g.f. Sum_{d|M} (exp(d*x)-1)/d, M=15.
 * https://oeis.org/A141014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==0)?0 : (1 + 3^(n-1) + 5^(n-1) + 15^(n-1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 727 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* exponent is n-1, cast to seq_t for big_pow */
    seq_t exp = (seq_t)(n - 1);

    seq_t val = ipow(3, exp);
    val = val + ipow(5, exp);
    val = val + ipow(15, exp);

    return val + 1;
}
