/*
 * A043086  a(n)=(s(n)+3)/10, where s(n)=n-th base 10 palindrome that starts
 *          with 7.
 * https://oeis.org/A043086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/10 where s(n) is the n‑th base‑10 palindrome
 *            starting with digit 7
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 a value is
       irrelevant; return 0. */
    if (n == 0) return 0;

    /* Find the length L of the n‑th palindrome and the offset of n
       inside the block of palindromes of that length. */
    idx_t L = 1;
    idx_t cumulative = 0;
    for (;; ++L) {
        idx_t k = (L - 1) / 2;      /* number of free digits besides the
                                        leading 7 and the trailing 7 (if
                                        length > 1). */
        idx_t count = 1;
        for (idx_t i = 0; i < k; ++i) count *= 10;  /* 10^k */
        if (cumulative + count >= n) break;
        cumulative += count;
    }

    idx_t k      = (L - 1) / 2;
    idx_t offset = n - cumulative - 1;   /* zero‑based index within block */

    /* Build the digit array of the palindrome. */
    size_t Lsize = (size_t)L;
    uint8_t *digits = (uint8_t *)malloc(Lsize);
    if (!digits) return 0;               /* out of memory – unlikely */
    digits[0] = 7;                       /* leading digit */

    if (k > 0) {
        /* Extract the k free digits of 'offset' in reverse order. */
        uint8_t *half = (uint8_t *)malloc((size_t)k);
        if (!half) { free(digits); return 0; }
        idx_t tmp = offset;
        for (idx_t i = 0; i < k; ++i) {
            half[i] = tmp % 10;
            tmp /= 10;
        }
        /* Place the free digits after the leading 7 (most significant
           first, so we reverse the array). */
        for (idx_t i = 1; i <= k; ++i) {
            digits[i] = half[k - i];
        }
        free(half);
    }

    /* Mirror the left part to complete the palindrome. */
    for (idx_t i = k + 1; i < Lsize; ++i) {
        digits[i] = digits[Lsize - 1 - i];
    }

    /* Convert the digit array into an integer value. */
    seq_t s = 0;
    for (idx_t i = 0; i < Lsize; ++i) {
        s = s * 10 + digits[i];
    }
    free(digits);

    /* a(n) = (s + 3) / 10. */
    seq_t result = (s + (seq_t)3) / (seq_t)10;
    return result;
}
