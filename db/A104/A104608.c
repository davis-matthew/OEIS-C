/*
 * A104608  Write the natural numbers in base 4 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digit at position n*(n+1)/2 in the infinite stream of
 *            base‑4 digits of the natural numbers 1,2,3,… (the rightmost
 *            digit of each row in the triangle)
 * Range:     u8 n <= 18, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Position of the desired digit (1‑based) */
    seq_t p = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find the block of numbers having the same base‑4 length */
    seq_t cumulative = 0;      /* digits used up to previous block */
    seq_t pow4 = 1;            /* 4^(L‑1) for current block with length L */
    idx_t  L = 1;              /* current digit length */

    while (1) {
        seq_t blockDigits = (seq_t)3 * (seq_t)L * pow4;   /* 3⋅L⋅4^(L‑1) */
        if (cumulative + blockDigits >= p) break;
        cumulative += blockDigits;
        pow4 = pow4 * (seq_t)4;        /* advance to next power of 4 */
        ++L;
    }

    /* Offset of the desired digit within this block (0‑based) */
    seq_t offset = p - cumulative - 1;
    idx_t numIndex     = (idx_t)(offset / L);   /* which number in block */
    idx_t digitOffset  = (idx_t)(offset % L);   /* position inside that number (0‑based from left) */

    /* Number containing the digit */
    seq_t number = pow4 + (seq_t)numIndex;       /* first number of block is 4^(L‑1) */

    /* Compute the digit at position digitOffset (0‑leftmost) */
    seq_t power = 1;
    for (idx_t i = 0; i < L - digitOffset - 1; ++i)
        power *= (seq_t)4;

    seq_t digit = (number / power) % (seq_t)4;
    return digit;
}
