/*
 * A047295  Numbers that are congruent to {0, 1, 2, 4, 6} mod 7.
 * https://oeis.org/A047295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 7 * floor((n-1)/5) + {0,1,2,4,6}[(n-1) mod 5]
 * This generates the integers congruent to {0,1,2,4,6} mod 7.
 */
seq_t a(idx_t n)
{
    /* n >= 1.  For n < 1 the result is undefined. */
    idx_t t = n - 1;                 /* zero‑based index of the term   */
    idx_t block = t / 5;              /* number of complete 5‑term blocks */
    idx_t pos   = t % 5;              /* position inside the current block */

    static const seq_t offs[5] = {0, 1, 2, 4, 6};

    return (seq_t)block * 7 + offs[pos];
}
