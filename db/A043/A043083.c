/*
 * A043083  a(n)=(s(n)+6)/10, where s(n)=n-th base 10 palindrome that starts
 *          with 4.
 * https://oeis.org/A043083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+6)/10, where s(n) is the n-th base‑10 palindrome
 *            starting with 4
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
/* idx_t and seq_t are provided by the build */

seq_t a(idx_t n)
{
    /* n >= 1 by definition; behaviour for n == 0 is irrelevant */
    idx_t remaining = n;
    idx_t L = 1;                     /* current length candidate */

    /* Find the length L that contains the n-th palindrome */
    while (1) {
        idx_t count;
        if (L == 1 || L == 2) {
            count = 1;
        } else {
            bool even = (L & 1) == 0;
            idx_t exp = even ? (L >> 1) - 1 : (L - 1) >> 1;
            count = 1;
            for (idx_t i = 0; i < exp; ++i) {
                /* Avoid overflow: if count would exceed 'remaining', stop early */
                if (count > remaining / 10) {
                    count = remaining + 1;
                    break;
                }
                count *= 10;
            }
        }
        if (remaining <= count) break;        /* L found */
        remaining -= count;
        ++L;
    }

    idx_t offset0 = remaining - 1;            /* 0‑based index within this length */

    seq_t s;                                  /* the palindrome s(n) */
    if (L == 1) {
        s = 4;
    } else if (L == 2) {
        s = 44;
    } else {
        bool even = (L & 1) == 0;
        idx_t k = even ? (L >> 1) : ((L - 1) >> 1);
        idx_t len_needed = even ? (k - 1) : k;

        /* Extract the decimal digits of offset0, least significant first */
        unsigned char arr[70];
        idx_t tmp = offset0;
        for (idx_t i = 0; i < len_needed; ++i) {
            arr[i] = tmp % 10;
            tmp /= 10;
        }

        /* firstHalf holds digits[2..k] (k-1 digits) in correct order */
        idx_t firstHalfLen = k - 1;
        unsigned char firstHalf[70];
        for (idx_t i = 0; i < firstHalfLen; ++i) {
            firstHalf[i] = arr[len_needed - 1 - i];
        }

        seq_t val = 4;                       /* leading digit */

        /* Append the first half */
        for (idx_t i = 0; i < firstHalfLen; ++i) {
            val = val * 10 + firstHalf[i];
        }

        /* If odd length, append the middle digit */
        if (!even) {
            unsigned char middle = arr[0];
            val = val * 10 + middle;
        }

        /* Append the mirrored first half */
        for (idx_t i = 0; i < firstHalfLen; ++i) {
            val = val * 10 + firstHalf[firstHalfLen - 1 - i];
        }

        /* Final trailing 4 */
        val = val * 10 + 4;

        s = val;
    }

    /* a(n) = (s + 6) / 10 */
    return (s + 6) / 10;
}
