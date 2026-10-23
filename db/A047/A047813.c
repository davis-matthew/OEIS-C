/*
 * A047813  Largest palindromic substring of n.
 * https://oeis.org/A047813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest palindromic decimal substring of n.
   For a given integer n, return the maximum numeric value
   among all palindromic substrings of the decimal representation of n.
   (This is A047813.) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n into its decimal digits (forward order). */
    char digits[64];
    int len = 0;

    if (n == 0) {
        digits[len++] = '0';
    } else {
        /* n is an idx_t (uint64_t).  t is a seq_t to allow
           arithmetic with the result type. */
        seq_t t = (seq_t)n;
        /* Extract digits in reverse order. */
        while (t > 0) {
            digits[len++] = '0' + (int)(t % 10);
            t /= 10;
        }
        /* Reverse to obtain the normal forward order. */
        for (int i = 0; i < len / 2; ++i) {
            char tmp = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    /* Search all substrings and keep the maximum palindromic one. */
    seq_t maxVal = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = i; j < len; ++j) {
            /* Quick palindrome test. */
            int ii = i, jj = j;
            bool pal = true;
            while (ii < jj) {
                if (digits[ii] != digits[jj]) {
                    pal = false;
                    break;
                }
                ++ii;
                --jj;
            }
            if (!pal)
                continue;

            /* Compute the numeric value of digits[i..j]. */
            seq_t val = 0;
            for (int k = i; k <= j; ++k) {
                val = val * 10 + (seq_t)(digits[k] - '0');
            }
            if (val > maxVal)
                maxVal = val;
        }
    }
    return maxVal;
}
