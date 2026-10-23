/*
 * A131503  Nonnegative integers n for which cos(n) is positive.
 * https://oeis.org/A131503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131503 – integers n for which cos(n) > 0,
   computed by scanning successive integers and testing the cosine
   using the standard math library. */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many positive‑cosine values found */
    idx_t m = 0;          /* current integer being tested */

    while (count < n) {
        /* Treat cosine of the integer m (in radians). */
        if (cos((double)m) > 0.0) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }

    /* The function should never reach here for valid n >= 1. */
    return 0;
}
