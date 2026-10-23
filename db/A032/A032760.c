/*
 * A032760  Take list of squares, move left digit of each term to end of
 *          previous term.
 * https://oeis.org/A032760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A032760: “Take list of squares, move left digit of each term to end of previous term”.
   Recurrence:
     a(1) = 0                                  // the first square 0^2
     For n ≥ 2:
       Let s = n^2.  Let d be its leftmost decimal digit.
       a(n) = a(n‑1) * 10 + d               // move that digit to the end of the previous term
       After this, the square s is truncated by removing its leftmost digit
       (this truncated value becomes the new previous square for the next step).
   (The actual following calculations are performed exactly as described; see code.)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    seq_t prev = (seq_t)0;               /* s_0 = 0^2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* process square of index i‑1 */
        idx_t idx = i - 1;
        seq_t curr = (seq_t)idx * ((seq_t)idx);  /* curr = (i‑1)^2 */

        /* find leftmost digit of curr and the power of 10 needed to isolate it */
        seq_t pow = 1;
        seq_t tmp = curr;
        while (tmp >= (seq_t)10) {
            tmp /= (seq_t)10;
            pow *= (seq_t)10;
        }
        int d = (int)(curr / pow);   /* leftmost digit, fits in small int (0‑9) */

        /* update previous term by appending the digit */
        prev = prev * (seq_t)10 + (seq_t)d;

        /* truncate the current square by removing its leftmost digit */
        curr = curr - (seq_t)d * pow;

        /* if this is the requested index, return the updated previous term */
        if (i == n) return prev;

        /* the truncated current square becomes the new previous term for next iteration */
        prev = curr;
    }

    return (seq_t)0; /* unreachable, but keeps compiler happy */
}
