/*
 * A018851  a(n)^2 is smallest square beginning with n.
 * https://oeis.org/A018851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   digits, floor_sqrt, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) where a(n)^2 is the smallest square that begins with the
 * decimal representation of n.  The algorithm checks increasing counts
 * of trailing digits d until a square lies in the interval
 * [n·10^d, (n+1)·10^d – 1].  The square root of that square is the return value. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e) */
#include "../utils/floor_sqrt.h"  /* seq_t floor_sqrt(seq_t x) */
#include "../utils/digits.h"      /* int digits(idx_t x) */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 (the supplied sequence definition
       agrees with a(0) = 0). */
    if (n == 0)
        return (seq_t)0;

    seq_t base = (seq_t)n;    /* the prefix to be matched */

    for (idx_t d = 0; ; ++d) {
        seq_t pow10d = pow10(d);              /* 10^d  */
        seq_t low  = base * pow10d;           /* n·10^d      */
        seq_t high = (base + 1) * pow10d - 1; /* (n+1)·10^d – 1 */

        /* Find the smallest m with m^2 >= low. */
        seq_t m = floor_sqrt(low);
        if (m*m < low) ++m;

        /* Check if this m satisfies m^2 <= high. */
        seq_t m2 = m * m;
        if (m2 <= high)
            return m;  /* a(n) found */

        /* Otherwise increase the number of trailing digits d. */
    }
}
