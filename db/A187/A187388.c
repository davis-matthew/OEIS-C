/*
 * A187388  Floor(s*n), where s=1+sqrt(6)-sqrt(5); complement of A187387.
 * https://oeis.org/A187388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (1+sqrt(6)-sqrt(5)) * n )
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses only integer arithmetic and the provided fast integer
 * square‑root function.  The key identity is:
 *
 * floor((1+√6-√5)n) = n + floor(√6 n) - floor(√5 n) - δ,
 * where δ = 1 if frac(√6 n) < frac(√5 n) and δ = 0 otherwise.
 * The comparison of fractional parts is performed without floating point
 * by the inequalities derived below.
 *
 * Let a = floor(√6 n) and b = floor(√5 n).
 * Let t = a - b   (t ≥ 0).
 *
 * frac(√6 n) < frac(√5 n)  ⇔  20 t² n²  > (n² - t²)².
 *
 * Therefore
 *   result = n + t;
 *   if (t > 0 && 20 t² n² > (n² - t²)²) result--.
 *
 * All calculations are carried out in the seq_t type; no floating point
 * operations are used.  The function works for any n ≥ 1 and respects
 * the exactness requirements for arbitrary‑precision builds. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                /* n^2 */
    seq_t six_n2 = n2 * (seq_t)6;      /* 6 * n^2 */
    seq_t five_n2 = n2 * (seq_t)5;     /* 5 * n^2 */

    seq_t a_sqrt = floor_sqrt(six_n2); /* floor(√6 n) */
    seq_t b_sqrt = floor_sqrt(five_n2);/* floor(√5 n) */

    seq_t t = a_sqrt - b_sqrt;         /* a - b  (t ≥ 0) */

    seq_t result = sn + t;             /* n + t */

    /* Determine whether to subtract one based on the fractional parts */
    if ((idx_t)t > 0) {
        /* left  = 20 * t² * n² */
        seq_t left = (seq_t)20 * t * t * n2;

        /* diff  = n² - t² */
        seq_t diff = n2 - t * t;
        /* right = diff² */
        seq_t right = diff * diff;

        if (left > right) {
            result -= (seq_t)1;
        }
    }
    return result;
}
