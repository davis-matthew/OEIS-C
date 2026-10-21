/*
 * A092972  Row 7 of array in A288580.
 * https://oeis.org/A092972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n,7) = Product_{i=0}^{⌊2n/7⌋} (n – 7·i) with terms equal to 0
 *            excluded. That is a(n) = n(n‑7)(n‑14)(n‑21)⋯ , skipping the
 *            factor equal to 0
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 27, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t k = 7;
    idx_t limit = (n * 2) / k;          /* floor(2n/k) – value of i to iterate up to */
    seq_t res = 1;                      /* neutral element for multiplication */

    for (idx_t i = 0; i <= limit; ++i) {
        seq_t t = (seq_t)n - (seq_t)i * (seq_t)k;  /* current factor */
        if (t == 0)            /* skip the zero factor (occurs when n is a multiple of k) */
            continue;
        res *= t;              /* multiply the non‑zero factor */
    }
    return res;
}
