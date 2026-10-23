/*
 * A190753  a(n) = n + [nr/t] + [ns/t] + [nu/t] + [nv/t] + [nw/t], where
 *          r=sinh(x), s=cosh(x), t=tanh(x), u=csch(x), v=sech(x), w=coth(x),
 *          x=Pi/2.
 * https://oeis.org/A190753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·s⌋ + ⌊n·s²/r⌋ + ⌊n·s/r²⌋ + ⌊n/r⌋ + ⌊n·s²/r²⌋ where
 *            r = sinh(π/2), s = cosh(π/2)
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_const.h"

/* Constants computed from r = sinh(π/2) and s = cosh(π/2) */
static const long double C1 = 2.509178478740660600L;   /* s */
static const long double C2 = 2.735302361931932300L;   /* s² / r */
static const long double C3 = 0.474354370428630100L;   /* s / r² */
static const long double C4 = 0.434324182054212440L;   /* 1 / r */
static const long double C5 = 1.188749058230344300L;   /* s² / r² */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – values below 1 are irrelevant */

    unsigned long long base = (unsigned long long)n;
    seq_t result = (seq_t)base;    /* n */

    result += floor_const(n, C1);
    result += floor_const(n, C2);
    result += floor_const(n, C3);
    result += floor_const(n, C4);
    result += floor_const(n, C5);

    return result;
}
