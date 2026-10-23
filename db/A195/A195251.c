/*
 * A195251  Number of lower triangles of a 5X5 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by two or less.
 * https://oeis.org/A195251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 92031109*n - 223153377 for n > 6. The first six terms are
 *            pre‑computed
 * Range:     u8 none, u16 n <= 1, u32 n <= 49, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t precomp[7] = {
    0ULL,                            /* unused, index 0 */
    (seq_t)32768ULL,                 /* a(1) */
    (seq_t)14348907ULL,              /* a(2) */
    (seq_t)67379894ULL,              /* a(3) */
    (seq_t)147824001ULL,             /* a(4) */
    (seq_t)237368212ULL,             /* a(5) */
    (seq_t)329060365ULL              /* a(6) */
};

seq_t a(idx_t n)
{
    if (n <= 6) {
        return precomp[(size_t)n];
    }
    seq_t term = (seq_t)n * (seq_t)92031109u;
    term -= (seq_t)223153377u;
    return term;
}
