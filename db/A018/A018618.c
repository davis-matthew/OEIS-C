/*
 * A018618  Divisors of 736.
 * https://oeis.org/A018618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th divisor of 736 in ascending order. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Divisors of 736 = 2^5 * 23. */
    static const seq_t divisors[12] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)8,
        (seq_t)16,
        (seq_t)23,
        (seq_t)32,
        (seq_t)46,
        (seq_t)92,
        (seq_t)184,
        (seq_t)368,
        (seq_t)736
    };

    if (n >= 1 && n <= 12) {
        return divisors[(size_t)(n - 1)];
    }
    return 0;
}
