/*
 * A095450  a(n) = 29 written in base n.
 * https://oeis.org/A095450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal integer obtained by writing 29 in base n and
 *            concatenating the base‑n digits as decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 14, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Special case: unary base (n = 1). 29 is represented by
       29 ones.  Append the digit 1 twenty‑nine times. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 29; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* For bases greater than 29 the single digit 29 suffices. */
    if (n > 29)
        return 29;

    /* Compute the base‑n digits of 29 (least significant first). */
    int digit[32];           /* maximum number of digits for any n. */
    int dcnt = 0;
    idx_t v = 29;
    while (v > 0) {
        int d = v % n;     /* 0 <= d <= 28 */
        digit[dcnt++] = d;
        v = v / n;
    }

    /* Concatenate the digits in most significant order to form the
       desired decimal number.  Each digit 0‑9 is one decimal digit,
       digits 10‑29 are two decimal digits. */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        int d = digit[i];
        res = res * ((d < 10) ? 10 : 100) + d;
    }
    return res;
}
