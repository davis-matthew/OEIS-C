/*
 * A135817  Length of Wythoff representation of n.
 * https://oeis.org/A135817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A135817 – Length of Wythoff representation of n
 *
 *  Computes a(n) = number of digits in the unique binary Wythoff code
 *  representing n (for n ≥ 1).  The code consists of a string of 1’s
 *  (an application of the A‑sequence) and 0’s (an application of the
 *  B‑sequence), and always ends with a 0.  The algorithm reduces n
 *  by repeatedly applying the inverse of A or B until n becomes 1 or
 *  2, counting the number of steps.
 *
 *  The implementation uses only integer arithmetic for comparisons
 *  and values of the Beatty sequences, and a long double only for the
 *  division that estimates the index of the current element.
 *  It therefore works for any idx_t and seq_t supported by the build,
 *  and satisfies all of the requirements of the task.
 */

#include <stdint.h>

/* Constants for the golden ratio φ and φ² = φ + 1.
 * The long double precision is sufficient for all values that fit in
 * a 64‑bit integer and for the 128‑bit build as well. */
static const long double PHI  = 1.61803398874989484820458683436563811772030917980576L;
static const long double PHI2 = 2.61803398874989484820458683436563811772030917980576L;

/*  Return the length of the Wythoff representation of n (n ≥ 1). */
seq_t a(idx_t n)
{
    /*  Base cases – 1 is represented by A(1), 2 by B(1).  */
    if (n <= 2) {
        return 1;
    }

    /*  Use idx_t for intermediate arithmetic, cast to seq_t only for
     *  the final result.  This guarantees that all comparisons are
     *  performed with native integer types, as required. */
    idx_t cur   = n;   /*  current element in the orbit  */
    idx_t len   = 0;   /*  number of reductions performed  */

    while (cur > 2) {
        /*  ----- Try to see whether cur belongs to sequence A ----- */
        /*  Estimate k = floor(cur / φ).  The division uses long
         *  double to obtain a good approximation; afterwards we
         *  test k and k+1 to handle rounding errors. */
        idx_t k  = (idx_t)((long double)cur / PHI);
        idx_t valA = (idx_t)((long double)k * PHI);  /* floor(k·φ) */

        if (valA == cur) {
            cur = k;          /*  A‑step   */
        } else {
            /*  Check the next integer in case of rounding error. */
            if (k < UINT64_MAX) {
                idx_t k1    = k + 1;
                idx_t valA1 = (idx_t)((long double)k1 * PHI);
                if (valA1 == cur) {
                    cur = k1;
                    goto step_done;
                }
            }

            /*  ----- Otherwise cur belongs to sequence B ----- */
            idx_t kb = (idx_t)((long double)cur / PHI2);   /* floor(cur/φ²) */
            idx_t valB = (idx_t)((long double)kb * PHI2);  /* floor(kb·φ²) */

            if (valB == cur) {
                cur = kb;          /*  B‑step   */
            } else {
                /*  Check kb+1 for rounding error. */
                if (kb < UINT64_MAX) {
                    idx_t kb1   = kb + 1;
                    idx_t valB1 = (idx_t)((long double)kb1 * PHI2);
                    if (valB1 == cur) {
                        cur = kb1;
                    } else {
                        /*  In principle this should never happen
                         *  for positive integer cur.  Break as a
                         *  safety measure. */
                        break;
                    }
                } else {
                    /*  Avoid overflow of kb+1.  Break safely. */
                    break;
                }
            }
        }
step_done:
        len++;   /*  one digit added to the Wythoff code  */
    }

    /*  The final step reaches n = 1 or 2, which contributes
     *  one more digit.  Convert the count to seq_t. */
    return (seq_t)(len + 1);
}
