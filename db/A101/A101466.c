/*
 * A101466  Create a histogram of the digits used so far in the sequence. Add
 *          to the last element of the sequence the most used digit on the
 *          histogram. If two or more digits have the same score in the
 *          histogram, add the highest. 0 is to be interpreted as 10 and so
 *          comes after 9. So if 0 is the digit to add,...
 * https://oeis.org/A101466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is produced iteratively: start with 1; for each step append
 *            the most frequent digit seen in the decimal representations of
 *            all previous terms (ties resolved by choosing the largest digit,
 *            where digit 0 is treated as 10). The new term is the previous
 *            term plus this chosen digit
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements that definition. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term of A101466, with n>=1. */
seq_t a(idx_t n)
{
    /* static state persists between calls */
    static seq_t *seq = NULL;      /* array of terms */
    static idx_t len = 0;          /* number of terms stored */
    static idx_t cap = 0;          /* capacity of array */
    static idx_t hist[10] = {0};   /* histogram of decimal digits 0-9 */

    /* Ensure capacity is enough for n terms */
    if (cap < n) {
        idx_t newcap = cap ? cap * 2 : 16;
        while (newcap < n) newcap *= 2;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Generate terms until we have at least n terms */
    while (len < n) {
        if (len == 0) {
            /* First term is 1 */
            seq[0] = (seq_t)1;
            hist[1] += 1;
        } else {
            /* Determine most frequent digit, tie → largest (0 treated as 10) */
            int bestDigit = 0;
            idx_t maxCount = hist[0];
            for (int d = 1; d < 10; ++d) {
                idx_t cnt = hist[d];
                if (cnt > maxCount ||
                    (cnt == maxCount && (d == 0 ? 10 : d) > (bestDigit == 0 ? 10 : bestDigit))) {
                    maxCount = cnt;
                    bestDigit = d;
                }
            }
            int add = (bestDigit == 0) ? 10 : bestDigit;
            seq[len] = seq[len - 1] + (seq_t)add;

            /* Update histogram with digits of the new term */
            seq_t tmp = seq[len];
            if (tmp == (seq_t)0) {
                hist[0] += 1;
            } else {
                while (tmp > (seq_t)0) {
                    int digit = (int)(tmp % (seq_t)10);
                    hist[digit] += 1;
                    tmp = tmp / (seq_t)10;
                }
            }
        }
        len++;
    }

    return seq[n - 1];
}
