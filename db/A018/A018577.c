/*
 * A018577  Divisors of 658.
 * https://oeis.org/A018577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 658 in ascending order. */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Static array will hold the eight divisors once they are generated. */
    static seq_t divisors[8];
    static int initialized = 0;

    if (!initialized) {
        /* Prime factors of 658: 2 * 7 * 47.  Each exponent is 0 or 1. */
        const seq_t f[3] = { (seq_t)2, (seq_t)7, (seq_t)47 };

        size_t k = 0;
        /* Enumerate all 2^3 = 8 combinations of the exponents. */
        for (int mask = 0; mask < 8; ++mask) {
            seq_t d = (seq_t)1;
            if (mask & 1) d *= f[0];
            if (mask & 2) d *= f[1];
            if (mask & 4) d *= f[2];
            divisors[k++] = d;
        }

        /* Simple selection sort – 8 items only, no external compare function needed. */
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = i + 1; j < 8; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        initialized = 1;
    }

    /* For n out of range, return 0 – the sequence is only defined for n = 1 … 8. */
    if (n < 1 || n > 8) return (seq_t)0;
    return divisors[n - 1];
}
