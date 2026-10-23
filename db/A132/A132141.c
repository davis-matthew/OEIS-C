/*
 * A132141  Numbers whose ternary representation begins with 1.
 * https://oeis.org/A132141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (3^⌊log_3(2*n)⌋ - 1)/2 where 3^⌊log_3(2*n)⌋ is the
 *            greatest power of 3 not exceeding 2*n. This implements the OEIS
 *            A132141 sequence: numbers whose ternary representation begins
 *            with 1. The function works for any idx_t n >= 1 and for any
 *            seq_t type (uint64_t, unsigned __int128, or an ar...
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* unused, but keeps compiler happy */

    /* 2 * n in seq_t */
    seq_t target = (seq_t)n * 2;

    /* largest power of 3 <= target */
    seq_t p = 1;
    while (p <= target / 3) {
        p *= 3;
    }

    /* a(n) = n + (p - 1)/2 */
    seq_t result = (seq_t)n + (p - 1) / 2;
    return result;
}
