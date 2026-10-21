/*
 * A056910  Numbers k such that 36*k^2 + 12*k + 7 is prime (sorted by absolute
 *          values with negatives before positives).
 * https://oeis.org/A056910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 18, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by iterative search over k sorted by absolute value
   (negative before positive), testing whether 36*k^2 + 12*k + 7 is prime.
   Returns the n‑th term of OEIS A056910. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t m = 0;          /* absolute value to test */
    idx_t count = 0;      /* number of valid terms found */

    while (1) {
        if (m == 0) {
            /* k = 0 */
            seq_t val = (seq_t)7;           /* 36*0^2 + 12*0 + 7 = 7 */
            if (is_prime_big(val)) {
                if (count == n) return (seq_t)0;
                count++;
            }
        } else {
            /* k = -m  (negative first) */
            seq_t k = -(seq_t)m;
            seq_t val = (seq_t)36 * k * k + (seq_t)12 * k + 7;
            if (is_prime_big(val)) {
                if (count == n) return k;
                count++;
            }

            /* k = +m  (positive second) */
            k = (seq_t)m;
            val = (seq_t)36 * k * k + (seq_t)12 * k + 7;
            if (is_prime_big(val)) {
                if (count == n) return k;
                count++;
            }
        }
        m++;
    }
}
