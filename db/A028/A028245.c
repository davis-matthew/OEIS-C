/*
 * A028245  a(n) = 5^(n-1) - 4*4^(n-1) + 6*3^(n-1) - 4*2^(n-1) + 1
 *          (essentially Stirling numbers of second kind).
 * https://oeis.org/A028245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1) - 4*4^(n-1) + 6*3^(n-1) - 4*2^(n-1) + 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 1427 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; safe guard for n == 0 */
    if (n == 0) return 0;

    /* exponent n-1 fits in idx_t and is cast to seq_t for big_pow */
    seq_t exp = (seq_t)(n - 1);

    /* Compute the necessary powers */
    seq_t p2 = ipow((seq_t)2, exp);   /* 2^(n-1) */
    seq_t p3 = ipow((seq_t)3, exp);   /* 3^(n-1) */
    seq_t p4 = ipow((seq_t)4, exp);   /* 4^(n-1) */
    seq_t p5 = ipow((seq_t)5, exp);   /* 5^(n-1) */

    /* Form the linear combination */
    seq_t res = p5;
    res += (seq_t)1;                      /* +1 */
    res += 6 * p3;                        /* +6*3^(n-1) */
    res -= 4 * p4;                        /* -4*4^(n-1) */
    res -= 4 * p2;                        /* -4*2^(n-1) */

    return res;
}
