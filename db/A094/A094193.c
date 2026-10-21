/*
 * A094193  Values y of the generator pairs (x, y), x>y of primitive
 *          Pythagorean triples, sorted on x.
 * https://oeis.org/A094193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the y–coefficient of the generator pair (x,y) of the
   primitive Pythagorean triples, sorted by the larger component x.
   For each integer x ≥ 2 the pairs (x,y) with 1 ≤ y < x, gcd(x,y)=1
   and x and y of opposite parity are generated.  The y values, in
   this order, form the sequence. */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0, harmless return */

    idx_t count = 0;

    for (idx_t x = 2; ; ++x) {
        for (idx_t y = 1; y < x; ++y) {
            if (((x - y) & 1) == 0)          /* same parity → skip */
                continue;
            if (gcd((seq_t)x, (seq_t)y) != 1)
                continue;
            ++count;
            if (count == n)
                return (seq_t)y;
        }
    }
}
