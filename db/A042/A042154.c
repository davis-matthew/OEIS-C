/*
 * A042154  Numerators of continued fraction convergents to sqrt(602).
 * https://oeis.org/A042154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1374*a(n-6) - a(n-12)
   Initial values: a(0)=24, a(1)=25, a(2)=49, a(3)=319, a(4)=368, a(5)=687,
   a(6)=33344, a(7)=34031, a(8)=67375, a(9)=438281, a(10)=505656, a(11)=943937 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Seed values up to n = 11 */
    static const seq_t init[12] = {
        24,   25,   49,   319,   368,   687,
        33344, 34031, 67375, 438281, 505656, 943937
    };

    /* Direct lookup for the first 12 terms */
    if (n < 12)
        return init[(size_t)n];

    /* Buffer to hold the last 12 computed values */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute terms from n = 12 up to the requested n */
    idx_t diff = n - 12;
    for (idx_t k = 0; k <= diff; ++k) {
        idx_t i   = 12 + k;
        size_t idx  = (size_t)(i % 12);        /* position of a(i) */
        size_t idx6 = (size_t)((i + 6) % 12);  /* a(i-6) */
        size_t idx12 = (size_t)((i + 12) % 12);/* a(i-12) */
        buf[idx] = 1374 * buf[idx6] - buf[idx12];
    }

    /* Return the desired term */
    return buf[(size_t)(n % 12)];
}
