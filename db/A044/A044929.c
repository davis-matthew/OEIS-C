/*
 * A044929  a(n)=so-se, where so(se)=sum of odd(even) base 7 run lengths of n.
 * https://oeis.org/A044929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum over runs r of (len(r) if len(r) is odd else –len(r))
 *            in the base‑7 expansion of n
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* n is 1‑based; n == 0 is not specified but handled gracefully. */
    if (n == (idx_t)0)
        return 0;

    idx_t m = n;                   /* working copy of n */
    unsigned char digits[64];      /* enough for any 64‑bit number in base 7 */
    size_t dcnt = 0;

    /* Convert to base 7, least significant digit first */
    while (m > 0) {
        digits[dcnt++] = (unsigned char)(m % 7);
        m /= 7;
    }

    /* Process runs from most significant digit to least */
    seq_t oddSum = 0, evenSum = 0, curLen = 1;
    size_t msd = dcnt - 1;                /* most significant digit index */
    unsigned char prev = digits[msd];

    for (size_t i = msd; i > 0; --i) {
        unsigned char cur = digits[i - 1];
        if (cur == prev) {
            curLen++;
        } else {
            if (curLen & 1)
                oddSum += curLen;
            else
                evenSum += curLen;
            prev = cur;
            curLen = 1;
        }
    }

    /* Final run */
    if (curLen & 1)
        oddSum += curLen;
    else
        evenSum += curLen;

    return oddSum - evenSum;
}
