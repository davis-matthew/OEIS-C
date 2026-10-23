/*
 * A133035  Divisors of 1810.
 * https://oeis.org/A133035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 1810 sorted ascending.
   a(1)=1, a(2)=2, a(3)=5, a(4)=10,
   a(5)=181, a(6)=362, a(7)=905, a(8)=1810. */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t divs[8] = {
        (seq_t)1,   (seq_t)2,   (seq_t)5,   (seq_t)10,
        (seq_t)181, (seq_t)362, (seq_t)905, (seq_t)1810
    };
    if (n >= 1 && n <= 8)
        return divs[(size_t)(n - 1)];
    return (seq_t)0;
}
