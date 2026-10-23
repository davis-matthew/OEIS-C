/*
 * A161347  Numbers which are equal to the product of their digits plus the
 *          sum of the cubes of their own digits.
 * https://oeis.org/A161347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th integer k such that k = prod_of_digits(k) +
 *            sum_of_cubes_of_digits(k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has only four terms: 31, 370, 407, 952. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t terms[4] = {0};
    static idx_t count = 0;
    static bool built = false;

    if (!built) {
        const idx_t LIMIT = 5000;          /* Above 4999 no further terms exist */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            idx_t tmp   = k;
            idx_t prod  = 1;
            idx_t sumc  = 0;
            if (tmp == 0) prod = 0;       /* handle zero, though never used here */
            while (tmp > 0) {
                idx_t d = tmp % 10;
                prod *= d;
                sumc += d * d * d;
                tmp  /= 10;
            }
            idx_t val = prod + sumc;
            if (val == k) {
                /* store in cache; we know at most four terms exist */
                terms[count] = (seq_t)k;
                ++count;
            }
        }
        built = true;
    }

    if (n == 0 || n > count) return 0;
    return terms[n - 1];
}
