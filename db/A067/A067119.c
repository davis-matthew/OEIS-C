/*
 * A067119  a(n) = floor[X/Y] where X = concatenation of first n even numbers
 *          in increasing order and Y = n-th triangular number.
 * https://oeis.org/A067119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor( concat(2,4,6,…,2n) / (n*(n+1)/2) )
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* triangular number */
    seq_t Y = (seq_t)n * ((seq_t)n + 1) / 2;

    seq_t q = 0;   /* quotient */
    seq_t r = 0;   /* remainder (0 <= r < Y) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ev = 2 * i;          /* even number */

        /* extract decimal digits of ev into an array (least significant first) */
        seq_t tmp = ev;
        seq_t digits[20];
        int dcnt = 0;
        do {
            digits[dcnt++] = tmp % 10;
            tmp /= 10;
        } while (tmp);

        /* process digits in normal order (most significant first) */
        for (int j = dcnt - 1; j >= 0; --j) {
            r = r * 10 + digits[j];   /* bring next digit into remainder */
            seq_t qdigit = r / Y;     /* quotient digit for this step */
            r %= Y;                   /* new remainder */
            q = q * 10 + qdigit;      /* accumulate quotient digit */
        }
    }

    return q;
}
