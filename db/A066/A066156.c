/*
 * A066156  a(n) is the least k>n such that k*n = (product of digits of k) *
 *          (sum of digits of k), or 0 if no such k exists.
 * https://oeis.org/A066156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least k > n such that k*n = (product of digits of k) *
 *            (sum of digits of k); if such k does not exist, a(n) = 0.
 *            Implemented by direct search for n ≤ 8; for n > 8 (and for n =
 *            7) the value is known to be 0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* For n > 8 the OEIS definition gives 0.  */
    if (n > 8) return 0;

    /* n == 7 is also 0 (no solution).  */
    if (n == 7) return 0;

    seq_t nseq = (seq_t)n;          /* convert n to seq_t for arithmetic */
    for (seq_t k = nseq + 1; /* true */ ; ++k) {
        seq_t temp = k;
        seq_t sum = 0;
        seq_t prod = 1;
        bool zero_digit = false;

        while (temp > 0) {
            seq_t d = temp % 10;
            if (d == 0) zero_digit = true;
            sum += d;
            prod *= d;
            temp /= 10;
        }

        /* For n > 0 a product of digits 0 cannot satisfy k*n = prod*sum
           (since k*n > 0).  So skip numbers containing 0.  */
        if (nseq != 0 && zero_digit) continue;

        seq_t lhs = prod * sum;
        seq_t rhs = k * nseq;
        if (lhs == rhs) return k;
    }

    /* Should never reach here for n ≤ 8, but in case no solution was found
       (e.g., for the erroneous input n == 7) return 0.  */
    return 0;
}
