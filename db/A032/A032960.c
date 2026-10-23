/*
 * A032960  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032960: numbers whose base‑10 representation has even digits at all odd
   positions (counting from the most significant digit as position 0). */

#include <stdbool.h>
#include <stddef.h>

/* Return true if x satisfies the digit property. */
static bool is_good(seq_t x)
{
    /* Use a fixed-size buffer sufficient for 128‑bit integers (~39 decimal digits). */
    unsigned int digits[64];
    size_t len = 0;

    if (x == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x != (seq_t)0) {
            digits[len++] = (unsigned int)(x % (seq_t)10);
            x /= (seq_t)10;
        }
    }

    /* digits[0] is least‑significant; check from most‑significant. */
    for (size_t i = 0; i < len; ++i) {
        unsigned int d = digits[len - 1 - i];
        if ((i & 1) && (d & 1))   /* odd index, odd digit? */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Find the nth term (1‑based). */
    idx_t count = 0;
    for (idx_t v = 1; ; ++v) {
        if (is_good((seq_t)v)) {
            ++count;
            if (count == n)
                return (seq_t)v;
        }
    }
}
