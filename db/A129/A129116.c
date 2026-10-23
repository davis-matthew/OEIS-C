/*
 * A129116  Multifactorial array: A(k,n) = k-tuple factorial of n, for
 *          positive n, read by ascending antidiagonals.
 * https://oeis.org/A129116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = multifactorial m!k where (k,m) are the antidiagonal
 *            coordinates of index n in the table A(k,m). The coordinates are
 *            determined by k + m = s, the smallest integer with (s‑1)*s/2 ≥ n
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   _antidiag_level
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_antidiag_level.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* defensive: outside the sequence */
        return (seq_t)0;

    uint64_t s      = _antidiag_level((uint64_t)n);
    uint64_t prev   = ((s - 2) * (s - 1)) / 2;        /* terms before this diagonal */
    uint64_t offset = (uint64_t)n - prev - 1;         /* position on the diagonal */

    uint64_t k = (s - 1) - offset;   /* k‑value (row index)  */
    uint64_t m = offset + 1;          /* n‑value (column index) */

    seq_t prod = 1;
    for (seq_t t = (seq_t)m; t > 0; ) {
        prod *= t;
        if (t <= (seq_t)k)           /* last factor reached */
            break;
        t -= (seq_t)k;
    }
    return prod;
}
