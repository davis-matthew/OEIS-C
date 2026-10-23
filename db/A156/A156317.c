/*
 * A156317  Numbers which are perfect squares and either form equal or larger
 *          perfect squares when reversed.
 * https://oeis.org/A156317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156317: perfect squares k² whose decimal reversal is also a
 * perfect square, and the reversed square is not smaller:
 * a(n) = k² where rev_decimal(k²) = m² with m ≥ k.  The function
 * iteratively tests successive k starting from 0 until the n‑th such
 * square is found.  All operations are performed on the generic
 * integer type seq_t, so the code works for 8‑, 16‑, 32‑, 64‑bit and
 * arbitrary‑precision builds. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* Return the decimal reverse of a number in seq_t. */
static seq_t
reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x   = x / 10;
    }
    return rev;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t
a(idx_t n)
{
    if (n == 0)             /* input below the offset; harmless */
        return 0;

    seq_t k   = 0;          /* current root to test */
    idx_t  cnt = 0;         /* number of qualifying squares found */

    while (true) {
        seq_t sq  = k * k;                          /* k²          */
        seq_t rev = reverse_decimal(sq);            /* rev(k²)     */
        seq_t t   = floor_sqrt(rev);                /* sqrt rev    */

        if (t * t == rev && t >= k) {               /* perfect square & m ≥ k */
            ++cnt;
            if (cnt == n)
                return sq;
        }
        ++k;                                       /* try next root */
    }
}
