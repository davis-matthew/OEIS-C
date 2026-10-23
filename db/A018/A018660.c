/*
 * A018660  Divisors of 812.
 * https://oeis.org/A018660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 812: list divisors of 812 in ascending order */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for divisors of 812 */
    static seq_t divisors[12];
    static size_t len = 0, cap = 0;
    static int initialized = 0;

    if (!initialized) {
        const seq_t N = (seq_t)812; /* The number whose divisors we list */
        seq_t large[32];            /* Temporary holder for large divisors */
        size_t large_len = 0;

        /* Find small divisors up to sqrt(N) */
        for (idx_t i = 1; (seq_t)i * (seq_t)i <= N; ++i) {
            seq_t d = (seq_t)i;
            if (N % d == 0) {
                divisors[len++] = d;
                if (d * d != N) {
                    large[large_len++] = N / d;
                }
            }
        }
        /* Append the large divisors in reverse order to keep sorting */
        for (size_t i = 0; i < large_len; ++i) {
            divisors[len++] = large[large_len - 1 - i];
        }
        cap = len;
        initialized = 1;
    }

    /* Return the requested divisor or 0 if out of bounds */
    return (n >= 1 && (size_t)(n - 1) < cap) ? divisors[n - 1] : (seq_t)0;
}
