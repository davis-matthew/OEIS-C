/*
 * A225904  Numerator of Sum_{k=1..n} 1/L(k) where L(n) is the n-th Lucas
 *          number (A000204).
 * https://oeis.org/A225904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/L(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where L(k) are the Lucas numbers) */
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safeguarded, n should be >= 1 */
    seq_t num = 0;                  /* current numerator of the sum */
    seq_t den = 1;                  /* current denominator of the sum */

    /* Lucas numbers: L1=1, L2=3, then Lk = L_{k-1} + L_{k-2} */
    seq_t prev2 = 1; /* L1 */
    seq_t prev1 = 3; /* L2 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t Lk;
        if (k == 1) {
            Lk = prev2; /* 1 */
        } else if (k == 2) {
            Lk = prev1; /* 3 */
        } else {
            Lk = prev1 + prev2;      /* compute next Lucas number */
            prev2 = prev1;           /* shift for next iteration */
            prev1 = Lk;
        }

        /* Add 1/Lk to the current fraction:  num/den + 1/Lk */
        seq_t new_num = num * Lk + den;
        seq_t new_den = den * Lk;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator of the reduced fraction */
}
