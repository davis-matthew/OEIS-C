/*
 * A018355  Divisors of 248.
 * https://oeis.org/A018355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 248: a(n) = the n‑th divisor of 248 in ascending order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 248 = 2^3 * 31^1; its 8 divisors (sorted ascending) are:
       1, 2, 4, 8, 31, 62, 124, 248.  */
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    idx_t count = 0;

    /* Outer loop over exponent of 31 (0 or 1). */
    for (idx_t e1 = 0; e1 <= 1; ++e1) {
        seq_t val1 = 1;
        for (idx_t i = 0; i < e1; ++i)
            val1 *= (seq_t)31;             /* 31^e1 */

        /* Inner loop over exponent of 2 (0..3). */
        for (idx_t e0 = 0; e0 <= 3; ++e0) {
            seq_t val = val1;
            for (idx_t i = 0; i < e0; ++i)
                val *= (seq_t)2;           /* 2^e0 */

            ++count;                       /* we have hit the next divisor */
            if (count == n)
                return val;
        }
    }

    /* If n > 8 the sequence is not defined; return 0. */
    return (seq_t)0;
}
