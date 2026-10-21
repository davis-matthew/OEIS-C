/*
 * A075151  a(n)=L(n)^2*C(n), L(n)=Lucas numbers (A000032), C(n)=reflected
 *          Lucas numbers (comment to A061084).
 * https://oeis.org/A075151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-1) + 6*a(n-2) + 3*a(n-3) - a(n-4), n > 3 Initial
 *            values: a(0)=8, a(1)=-1, a(2)=27, a(3)=-64
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function works for both integer and arbitrary‑precision seq_t
   types because it only uses +, -, * operators and small integer
   constants.  No special headers beyond <stdint.h> (required for
   uint64_t) are needed. */

#include <stdint.h>

/* Statically store the first four terms to avoid recomputing them. */
static const seq_t a0 = (seq_t)8;
static const seq_t a1 = (seq_t)-1;
static const seq_t a2 = (seq_t)27;
static const seq_t a3 = (seq_t)-64;

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Working variables for the recurrence. */
    seq_t prev4 = a0; /* a(i-4)  */
    seq_t prev3 = a1; /* a(i-3)  */
    seq_t prev2 = a2; /* a(i-2)  */
    seq_t prev1 = a3; /* a(i-1)  */
    seq_t next;       /* a(i)    */

    /* Compute successive terms until we reach n. */
    for (idx_t i = 4; i <= n; ++i) {
        next = ((seq_t)-3) * prev1
             + ((seq_t) 6) * prev2
             + ((seq_t) 3) * prev3
             - prev4;

        /* Shift the window for the next iteration. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1; /* a(n) is stored in prev1 after the loop */
}
