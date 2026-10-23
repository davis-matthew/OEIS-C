/*
 * A044932  a(n)=so-se, where so(se)=sum of odd(even) base 10 run lengths of
 *          n.
 * https://oeis.org/A044932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of run lengths of the decimal representation of n
 *            with a plus sign for odd lengths and a minus sign for even
 *            lengths. That is: a(n) = Σ (len_i % 2 ? len_i : -len_i) over all
 *            runs of equal consecutive digits of n
 * Range:     i8 n <= 87, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    unsigned char digits[20];
    size_t len = 0;

    /* Extract digits in reverse order */
    if (temp == 0) {
        digits[len++] = 0;
    } else {
        while (temp > 0) {
            digits[len++] = (unsigned char)(temp % 10);
            temp /= 10;
        }
    }

    seq_t runLen = 0;
    int prevDigit = -1;

    /* Scan digits from most significant to least significant */
    for (int i = (int)len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d == prevDigit) {
            runLen++;
        } else {
            if (prevDigit != -1) {         /* finished a run */
                if (runLen & 1)
                    sum += runLen;        /* odd length */
                else
                    sum -= runLen;        /* even length */
            }
            prevDigit = d;
            runLen = 1;
        }
    }

    /* Add the final run */
    if (runLen > 0) {
        if (runLen & 1)
            sum += runLen;
        else
            sum -= runLen;
    }

    return sum;
}
