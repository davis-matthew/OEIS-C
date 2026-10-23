/*
 * A044266  Numbers n such that string 1,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Find the nth positive integer whose base‑9 representation
   contains the digit sequence “16” (digit 1 followed by digit 6) but whose
   predecessor does not contain that sequence.  The sequence is A044266. */
#include <stdbool.h>

/* Return 1 if the base‑9 representation of x contains the consecutive
   digits 1,6 (from most significant to least significant). */
static bool has16(seq_t x)
{
    seq_t prev = (seq_t)0;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)9;      /* least significant digit   */
        if (prev == (seq_t)6 && d == (seq_t)1) /* pattern 6,1 found → 1,6 in normal order */
            return true;
        prev = d;
        x /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;      /* offset 1, but guard against 0 */
    seq_t count = 0;
    seq_t num   = 0;                  /* start from 0 */
    bool prev_has16 = false;          /* has16(0) == false */

    while (count < n) {
        num++;
        bool cur_has16 = has16(num);
        if (cur_has16 && !prev_has16) {
            count++;
            if (count == n)
                return num;
        }
        prev_has16 = cur_has16;
    }
    return num;   /* unreachable */
}
