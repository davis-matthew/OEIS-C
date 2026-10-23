/*
 * A133852  a(n) = A000040(n) - A133578(n).
 * https://oeis.org/A133852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000040(n) - A133578(n)
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 5, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computes a(n) = nth prime  –  A133578(n).
   A133578(1)=4; for n≥2, A133578(n)=A133578(n-1)+2 if n is even,
   else A133578(n-1)+3. */
seq_t a(idx_t n)
{
    if (n < 1)          /* offset is 1; undefined for n<1, return 0 */
        return (seq_t)0;

    /* --- compute nth prime ------------------------------------ */
    idx_t    count = 0;
    seq_t    prime = 0;
    for (idx_t cand = 2;; ++cand) {
        if (is_prime(cand)) {
            ++count;
            if (count == n) {
                prime = cand;   /* implicit conversion to seq_t */
                break;
            }
        }
    }

    /* --- compute A133578(n) ----------------------------------- */
    seq_t val = 4;                     /* A133578(1) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0)              /* even i : +2 */
            val += (seq_t)2;
        else                           /* odd i  : +3 */
            val += (seq_t)3;
    }

    /* --- result ----------------------------------------------- */
    return prime - val;
}
