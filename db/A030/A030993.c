/*
 * A030993  8-automorphic numbers: final digits of 8*n^2 agree with n.
 * https://oeis.org/A030993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the unique integer < 10^n such that a(n) is divisible by
 *            2^n and 8*a(n) - 1 is divisible by 5^n. The routine builds a(n)
 *            iteratively by extending the previous term with one decimal
 *            digit on the left
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for the first term.  The offset of the
       sequence is 1, so a(1) = 2. */
    if (n <= 1)
        return (seq_t)2;

    /* a_prev holds the current a(i-1).  Start with a(1). */
    seq_t a_prev = (seq_t)2;

    /* Pre‑compute the powers needed for i = 2 */
    seq_t pow10 = (seq_t)10;   /* 10^(i-1) for i = 2 */
    seq_t pow2  = (seq_t)4;    /* 2^i for i = 2 */
    seq_t pow5  = (seq_t)25;   /* 5^i for i = 2 */

    /* Iterate i = 2 .. n, constructing a(i) each step. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t new_a = 0;

        /* Try all decimal digits d so that
           d*10^(i-1)+a_prev satisfies the two congruences. */
        for (int d = 0; d <= 9; ++d) {
            seq_t cand = (seq_t)d * pow10 + a_prev;

            /* Condition 1: cand divisible by 2^i */
            if (cand % pow2 != 0)
                continue;

            /* Condition 2: 8*cand ≡ 1 (mod 5^i) */
            seq_t mod5 = cand % pow5;
            seq_t r = (mod5 * (seq_t)8) % pow5;
            if (r != (seq_t)1)
                continue;

            new_a = cand;
            break;  /* unique digit found */
        }

        a_prev = new_a;

        /* Update the powers for the next i (if any).  The
           loop multiplies by the base to reach 2^(i+1),
           5^(i+1) and 10^i for the next iteration. */
        if (i < n) {
            pow10 *= (seq_t)10;
            pow2  *= (seq_t)2;
            pow5  *= (seq_t)5;
        }
    }

    return a_prev;
}
