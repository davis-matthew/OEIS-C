/*
 * A138377  a(0) = 0, a(1) = 1, a(2) = 3, a(3) = 2; thereafter a(n) =
 *          -4*a(n-4).
 * https://oeis.org/A138377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(3)=2, a(n) = -4*a(n-4) for n≥4
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 61, i64 n <= 125,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used by idx_t (provided by build) */

seq_t a(idx_t n)
{
    /* 4‑cycle initial values */
    static const seq_t init_vals[4] = {0, 1, 3, 2};
    seq_t init = init_vals[n & 3U];          /* n % 4 (unsigned) */

    /* Exponent for (-4) : floor(n/4) */
    idx_t exp = n >> 2U;                               /* n / 4 */

    /* Compute (-4)^exp by binary exponentiation, using seq_t arithmetic */
    seq_t base = (seq_t)-4;
    seq_t power = (seq_t)1;

    while (exp != 0) {
        if (exp & 1U)          /* odd bit: multiply into the result */
            power *= base;

        base *= base;          /* square the base for the next bit */
        exp >>= 1U;            /* move to the next bit */
    }

    return init * power;       /* final value a(n) */
}
