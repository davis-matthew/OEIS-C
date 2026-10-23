/*
 * A044412  Numbers n such that string 8,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044412: numbers n such that the decimal representation of n
   contains the substring "80" but that of n-1 does not.  The function
   computes the n-th term by a brute‑force search. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not used but harmless */

    idx_t   count = 0;
    seq_t   num   = (seq_t)80;              /* first candidate */

    while (1) {
        /* check whether num contains the pattern 8 followed by 0
           (in the normal left‑to‑right order).  In reverse digit
           scanning we detect it when a digit 8 is seen and the
           immediately less significant digit was 0. */
        bool has80 = false;
        seq_t temp = num;
        seq_t prev = (seq_t)10;             /* sentinel >9 */

        while (temp > 0) {
            unsigned long long d = (unsigned long long)(temp % 10);
            if (d == 8ULL && prev == (seq_t)0) { has80 = true; break; }
            prev = temp % 10;
            temp /= 10;
        }

        if (has80) {
            /* verify that the predecessor does not contain 80 */
            seq_t prev_num = num - 1;
            bool prevHas80 = false;
            temp = prev_num;
            prev = (seq_t)10;

            while (temp > 0) {
                unsigned long long d = (unsigned long long)(temp % 10);
                if (d == 8ULL && prev == (seq_t)0) { prevHas80 = true; break; }
                prev = temp % 10;
                temp /= 10;
            }

            if (!prevHas80) {
                ++count;
                if (count == n) return num;
            }
        }
        ++num;
    }
}
