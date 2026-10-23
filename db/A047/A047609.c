/*
 * A047609  Numbers that are congruent to {0, 4, 5} mod 8.
 * https://oeis.org/A047609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*ceil(n/3) - (3 if n mod3==0, 8 if n mod3==1, 4 if n
 *            mod3==2)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the OEIS sequence A047609: numbers congruent to {0,4,5} mod 8. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    unsigned rem = (unsigned)(n % 3);
    /* ceil(n/3) = (n+2)/3 for unsigned arithmetic */
    seq_t k = (seq_t)((n + 2) / 3);
    seq_t sub;
    switch (rem) {
    case 0: sub = (seq_t)3; break;
    case 1: sub = (seq_t)8; break;
    default: sub = (seq_t)4; break;
    }
    return (k << 3) - sub;
}
