/*
 * A040361  Continued fraction for sqrt(381).
 * https://oeis.org/A040361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of sqrt(381)
   a(0) = floor(sqrt(381)) = 19
   a(n) for n>=1 are the periodic terms: 1,1,12,1,1,38 repeated */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* constants for sqrt(381) */
    const uint64_t D   = 381;       /* the radicand */
    const uint64_t a0  = 19;       /* floor(sqrt(D)) */
    const uint64_t twoa0 = a0 * 2; /* value that ends the period */

    /* static storage for the periodic part; computed once */
    static seq_t period[200];
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        uint64_t m = 0;   /* m */
        uint64_t d = 1;   /* d */
        uint64_t a = a0;  /* current a (not stored) */

        /* generate period until the first appearance of a==2*a0 */
        size_t idx = 0;
        for (;;) {
            /* next triple (m,d,a) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;          /* floor division */

            period[idx++] = (seq_t)a;

            if (a == twoa0)            /* terminating condition: 2*a0 reached */
                break;
        }
        period_len = idx;
        initialized = 1;
    }

    if (n == 0)
        return (seq_t)a0;

    /* retrieve from periodic part */
    size_t pos = (size_t)(n - 1) % period_len;
    return period[pos];
}
