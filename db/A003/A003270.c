/*
 * A003270  A nonrepetitive sequence.
 * https://oeis.org/A003270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Arshon's square-free word, the fixed point starting with 1 of
 *            the map where the letter k at an odd position becomes k,k+1,k+2
 *            and at an even position becomes k+2,k+1,k (letters taken mod 3,
 *            in 1..3). With n-1 = 3q + r (0 <= r < 3) and c = a(q+1): a(n) =
 *            ((c-1) + r) mod 3 + 1 if q+1 is odd, a(n) =...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned arshon(idx_t n)
{
    if (n <= 1) return 1;
    idx_t m = n - 1;
    idx_t q = m / 3;
    unsigned r = (unsigned)(m % 3);
    unsigned c = arshon(q + 1);
    unsigned off = (q % 2 == 0) ? r : 2 - r;
    return ((c - 1) + off) % 3 + 1;
}

seq_t a(idx_t n)
{
    return (seq_t)arshon(n);
}
