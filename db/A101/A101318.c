/*
 * A101318  Numbers n such that Sum_of_Digits modulo n <= 2.
 * https://oeis.org/A101318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth natural number whose sum of digits ≤ 2 (i.e.,
 *            sum_of_digits(n) ≤ 2). The sequence is the list of all positive
 *            integers with digit sum ≤ 2 in increasing order
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find the decimal length L of the answer.  For L = 1 the
     * numbers are 1..9, for L ≥ 2 there are exactly L+1 numbers
     * of that length.  */
    idx_t L = 1;
    idx_t total = 9;             /* numbers ≤ length 1 */
    while (total < n) {
        ++L;
        total += (L + 1);        /* add count of numbers of length L */
    }

    /* k is the index within the block of numbers of length L (1‑based). */
    idx_t k;
    if (L == 1)
        k = n;                  /* 1‑9 only */
    else {
        idx_t prev_total = total - (L + 1);  /* numbers before this block */
        k = n - prev_total;                  /* 1 … L+1 */
    }

    /* Construct the kth number of length L. */
    if (L == 1) {
        return (seq_t)k;        /* 1 … 9 */
    }

    if (k == 1) {
        /* 1 followed by L-1 zeros */
        return pow10(L - 1);
    }

    if (k <= L) {
        /* k ≥ 2: place a single '1' among the remaining L-1 digits.
         * The exponent of the extra '1' is (k-2): 0 for the least
         * significant position, up to L-2 for the most significant
         * among the trailing digits. */
        return pow10(L - 1) + pow10(k - 2);
    }

    /* k == L+1 : the number 2 followed by L-1 zeros */
    return (seq_t)2 * pow10(L - 1);
}
