/*
 * A063929  Radius of A-excircle of Pythagorean triangle with a = (n+1)^2 -
 *          m^2, b = 2*(n+1)*m and c = (n+1)^2 + m^2.
 * https://oeis.org/A063929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (k+1)*(k-h), where * k = ceil((sqrt(8n+1)-1)/2) (the row
 *            index), * h = n - (k-1)k/2 - 1 (offset within the row). * This
 *            enumerates the values T(n,m) = (n+1)(n-m+1) for 1 ≤ m ≤ n * in
 *            row‑major order (row 1, row 2, …)
 * Range:     u8 n <= 7, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* for function floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Protect against n < 1 – the sequence is only defined for n ≥ 1. */
    if (n == 0) return 0;

    seq_t nn = n;
    /* Find k such that k(k+1)/2 ≥ n.  Solve k² + k – 2n ≥ 0. */
    seq_t s = floor_sqrt((seq_t)8 * nn + 1);      /* floor(sqrt(1+8n)) */
    seq_t k = (s - 1) / 2;                       /* floor of the root */

    /* Adjust k up or down until the inequality holds. */
    seq_t t = k * (k + 1) / 2;
    if (t < nn) {
        /* If k is too small, increase it. */
        do {
            ++k;
            t = k * (k + 1) / 2;
        } while (t < nn);
    } else {
        /* If k is too large, decrease it. */
        while (k > 0 && (k - 1) * k / 2 >= nn) {
            --k;
        }
    }

    /* Compute position h within the row (0‑based). */
    seq_t prev = (k - 1) * k / 2;
    seq_t h = nn - prev - 1;

    /* Radius value T(k, h+1) = (k+1)*(k - h). */
    seq_t radius = (k + 1) * (k - h);
    return radius;
}
