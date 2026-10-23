/*
 * A018650  Divisors of 795.
 * https://oeis.org/A018650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 795 in increasing order */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static divisor list */
    static const uint64_t divs[8] = {
        1ULL, 3ULL, 5ULL, 15ULL, 53ULL, 159ULL, 265ULL, 795ULL
    };

    if (n < 1)           /* safety for unexpected lower indices */
        return 0;

    idx_t idx = n - 1;   /* 0‑based position */

    if (idx < 8)
        return (seq_t)divs[(size_t)idx];

    return 0;            /* sequence terminates after 8 terms */
}
