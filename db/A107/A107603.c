/*
 * A107603  In the natural numbers, swap digits in pairs (even-odd).
 * https://oeis.org/A107603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: For the decimal representation of n, traverse digit pairs from
 * most‑significant to least‑significant.  If the first digit of a pair is
 * odd and the second digit is even, swap the two digits; otherwise keep
 * the order.  Any trailing single digit remains unchanged.
 * The resulting sequence is OEIS A107603. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1, but handle 0 safely */
        return 0;

    /* // maximum 20 decimal digits for uint64_t */
    seq_t digits[32];
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }

    seq_t res = 0;
    /* process from most significant digit to least */
    for (int i = len; i > 0; ) {
        if (i >= 2) {                 /* at least one full pair */
            seq_t first  = digits[i - 1];      /* more significant digit */
            seq_t second = digits[i - 2];      /* less significant digit */

            if ((first & 1) && ((second & 1) == 0)) { /* first odd, second even */
                res = res * 10 + second;
                res = res * 10 + first;
            } else {
                res = res * 10 + first;
                res = res * 10 + second;
            }
            i -= 2;
        } else {                        /* single trailing digit */
            res = res * 10 + digits[0];
            i--;
        }
    }
    return res;
}
