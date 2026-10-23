/*
 * A110743  Smallest prime beginning with n reversed.
 * https://oeis.org/A110743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = smallest prime whose decimal representation starts with *
 *            the decimal digits of n reversed (ignoring leading zeros)
 * Range:     u8 n <= 14, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isprime_big.h"
#include "../utils/pow10.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Reverse decimal digits of n (ignoring leading zeros). */
    idx_t rev = 0;
    idx_t t = n;
    while (t > 0) {
        rev = rev * 10 + (t % 10);
        t /= 10;
    }

    /* Search for the smallest prime that begins with this prefix. */
    for (idx_t extra_digits = 0; ; ++extra_digits) {
        /* low = rev * 10^extra_digits, high = (rev+1)*10^extra_digits - 1 */
        seq_t pow10d = pow10(extra_digits);
        seq_t low = (seq_t)rev * pow10d;
        seq_t high = ((seq_t)(rev + 1) * pow10d) - 1;

        seq_t cand = low;
        /* Start from the first odd candidate (except for 2). */
        if ((cand & 1) == 0 && cand != (seq_t)2) {
            cand += 1;
        }

        while (cand <= high) {
            if (isprime_big(cand)) {
                return cand;
            }
            cand += 2;
        }
        /* If no prime in this range, continue with more digits. */
    }
}
