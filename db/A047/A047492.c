/*
 * A047492  Numbers that are congruent to {0, 4, 5, 7} mod 8.
 * https://oeis.org/A047492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047492: Numbers congruent to {0,4,5,7} mod 8.
   Formula: a(n) = 8 * ((n-1)/4) + {0,4,5,7}[(n-1)%4]   (for n>=1)
*/

static const seq_t _offsets[4] = {0,4,5,7};

seq_t a(idx_t n)
{
    /* n is 1‑based and guaranteed to be ≥1 */
    idx_t idx   = n - 1;          /* zero‑based index */
    idx_t block = idx / 4;        /* number of complete 8‑number blocks before the current element */
    idx_t pos   = idx % 4;        /* position inside the block */
    return (seq_t)8 * (seq_t)block + _offsets[pos];
}
