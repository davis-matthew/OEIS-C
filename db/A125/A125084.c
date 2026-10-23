/*
 * A125084  Cubes which have a partition as the sum of 3 squares.
 * https://oeis.org/A125084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000378(n)^3 (A000378(n) is the n‑th integer
 *            representable as a sum of three squares; the sequence starts
 *            with 0)
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 1358, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x is of the form 4^k*(8m+7) (and hence not
   representable as a sum of three squares).  Zero is a special
   case that is representable. */
static inline bool is_excluded(seq_t x)
{
    if (x == 0)
        return false;          /* 0 = 0^2+0^2+0^2 */

    seq_t y = x;
    /* strip powers of 4 */
    while ((y % 4) == 0)
        y = y / 4;
    /* now y % 8 == 7 signals exclusion */
    return (y % 8) == 7;
}

/* Return the n-th element of A125084.
   Indexing is 1‑based: a(1)=0, a(2)=1^3, ...            */
seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (undefined behaviour for n<1) */
    if (n == 0)
        return 0;

    idx_t found = 0;   /* how many admissible numbers seen so far */
    seq_t m = 0;       /* current integer being tested (includes 0) */

    while (1) {
        if (!is_excluded(m)) {
            ++found;
            if (found == n)
                break;
        }
        ++m;
    }

    /* Compute m^3 carefully to avoid intermediate overflow
       (for integral seq_t types this is safe for the bounds tested). */
    seq_t sq = m * m;   /* m^2 */
    seq_t cube = sq * m;/* m^3 */
    return cube;
}
