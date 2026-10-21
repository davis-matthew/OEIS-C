/*
 * A221672  Length of shortest non-constant arithmetic progression (AP)
 *          containing n squares.
 * https://oeis.org/A221672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A221672: Length of shortest non‑constant AP containing n squares.
   For n = 1, 2, 3 the answer is n.
   For n = 4 the answer is 5.
   For n ≥ 5 let j = n – 1.
   - If j is odd: let m = (j + 1) / 2,  a(n) = m*(3*m - 1)/2 + 1.
   - If j is even: let m =  j / 2,   a(n) = m*(3*m + 1)/2 + 1.
   This formula follows from a(n) – 1 being the (n‑1)st generalized pentagonal number.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small n cases */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)5;
        }
    }

    /* n >= 5 */
    idx_t j = n - 1;          /* j = n‑1  (j >= 4) */
    seq_t m, value;

    if (j & 1) {                /* j odd  -> use (3*m - 1) formula */
        m = (seq_t)(j + 1) / 2;
        value = m * ((seq_t)3 * m - 1) / 2;
    } else {                    /* j even -> use (3*m + 1) formula */
        m = (seq_t)(j / 2);
        value = m * ((seq_t)3 * m + 1) / 2;
    }

    return value + (seq_t)1;
}
