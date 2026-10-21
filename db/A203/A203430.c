/*
 * A203430  Vandermonde determinant of the first n numbers
 *          (1,3,4,6,7,9,10,...) = (j+floor(j/2)).
 * https://oeis.org/A203430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (⌊3j/2⌋ - ⌊3i/2⌋)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Vandermonde determinant of the first n numbers
   1,3,4,6,7,9,10,12,13,15,16,18,...  */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined only for n≥1 */

    /* Counts of the two arithmetic progressions:
       S1 : numbers ≡1 (mod 3)  : m1 elements
       S2 : numbers ≡0 (mod 3)  : m2 elements  */
    const idx_t m1 = (n + 1) / 2;  /* ceil(n/2) */
    const idx_t m2 = n / 2;        /* floor(n/2) */

    /* Product of factorials inside each progression */
    seq_t result = (seq_t)1;
    seq_t fact   = (seq_t)1;

    /* Inside S1 */
    for (idx_t k = 1; k < m1; ++k) {
        fact *= (seq_t)k;
        result *= fact;
    }

    /* Inside S2 */
    fact = (seq_t)1;
    for (idx_t k = 1; k < m2; ++k) {
        fact *= (seq_t)k;
        result *= fact;
    }

    /* Powers of 3 from the spacing within each progression */
    const idx_t exp3 = (m1 * (m1 - 1)) / 2 + (m2 * (m2 - 1)) / 2;
    if (exp3) result *= seq_pow((seq_t)3, exp3);

    /* Cross terms where an S1 element is smaller: 3*t - 1  */
    for (idx_t t = 1; t <= m2; ++t) {
        idx_t cnt = (t <= m2) ? ((m2 - t + 1 < m1) ? (m2 - t + 1) : m1) : 0;
        if (cnt == 0) continue;
        result *= seq_pow((seq_t)(3 * t - 1), cnt);
    }

    /* Cross terms where an S2 element is smaller: 3*t + 1  */
    for (idx_t t = 1; t < m1; ++t) {
        idx_t limit = (m1 - 1) - t;
        if (limit < 1) continue;
        idx_t cnt = (limit < m2) ? limit : m2;
        if (cnt == 0) continue;
        result *= seq_pow((seq_t)(3 * t + 1), cnt);
    }

    return result;
}
