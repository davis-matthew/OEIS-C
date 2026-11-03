/*
 * A199478  Irregular triangle read by rows: T(n,i) =
 *          2^(i-1)*(binomial(n-i+1,i)+binomial(n-i,i)), n >= 1, 0 <= i <=
 *          (n+1)/2.
 * https://oeis.org/A199478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the row‑major listing of T(n,i) where T(n,i) =
 *            2^(i-1)*(binomial(n-i+1,i)+binomial(n-i,i)) for n>=1,
 *            0<=i<=(n+1)/2
 * Range:     u8 n <= 37, u16 n <= 104, u32 n <= 350, u64 n <= 1209,
 *            u128 n <= 2650, big any n
 * Validated: standard, pass, 80 terms through n = 2436 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t target = n;          /* 1‑based index of desired term */
    idx_t count  = 0;          /* number of terms produced so far */

    for (idx_t row = 1; ; ++row) {
        idx_t max_i = (row + 1) / 2;   /* maximum i for this row */
        for (idx_t i = 0; i <= max_i; ++i) {
            /* Compute the two binomial coefficients */
            seq_t c1 = binomial(row - i + 1, i);
            seq_t c2 = binomial(row - i,     i);
            seq_t sum = c1 + c2;

            /* Multiply by 2^(i-1); for i==0 this is a division by 2 */
            seq_t value;
            if (i == 0) {
                value = sum >> 1;                 /* /2 using integer shift */
            } else {
                value = sum * pow2(i - 1);
            }

            ++count;
            if (count == target) {
                return value;
            }
        }
    }
}
