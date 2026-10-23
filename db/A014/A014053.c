/*
 * A014053  Inverse of 44th cyclotomic polynomial.
 * https://oeis.org/A014053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the coefficients of the formal power series
   1 / Φ₄₄(x), where Φ₄₄ is the 44th cyclotomic polynomial.
   The coefficients are periodic with period 44.  They are
   computed via the recurrence
      a(0) = 1
      a(n) = -∑_{k=1}^{20} p_k · a(n-k),   n ≥ 1
   where p_k are the coefficients of Φ₄₄(x):
      Φ₄₄(x) = 1        – x² + x⁴ – x⁶ + … + x²⁰.
   The array 'coeff' holds these values for 0 ≤ k ≤ 20.  Once the
   first 44 terms are computed they are cached and the sequence is
   returned as a(n mod 44).  This satisfies the periodicity
   requirement and ensures O(1) work after the initialisation. */

#include <stdbool.h>

static const int coeff[21] = {
    /* k = 0 .. 20 */
     1,  0, -1,  0,  1,  0, -1,  0,  1,  0,
    -1,  0,  1,  0, -1,  0,  1,  0, -1,  0,  1
};

seq_t a(idx_t n) {
    /* Cache the 44-periodic values after first use. */
    static bool initialized = false;
    static seq_t cache[44];

    if (!initialized) {
        /* a(0) = 1 */
        cache[0] = 1;
        /* Compute a(1) .. a(43) using the recurrence. */
        for (idx_t i = 1; i < 44; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 20; ++k) {
                if (i < k) continue;          /* a(n-k) = 0 for negative index */
                sum += (seq_t)coeff[k] * cache[i - k];
            }
            cache[i] = -sum;
        }
        initialized = true;
    }

    /* Periodicity: return a(n mod 44). */
    return cache[n % 44];
}
