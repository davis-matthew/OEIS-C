/*
 * A059729  Carryless squares n X n base 10.
 * https://oeis.org/A059729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = carryless square of n in base 10 a(n) is defined as the
 *            product of n with itself, where each digit is added/multiplied
 *            modulo 10 without any carry propagation
 * Range:     u8 n <= 19, u16 n <= 299, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine converts n to its decimal digits, computes the
   carryless convolution of the digit vectors, and then rebuilds
   the resulting number from its digits.  Complexity is O(d^2)
   with d = number of decimal digits of n. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle zero separately */
    if (n == 0)
        return 0;

    /* obtain decimal digits of n (least‑significant first) */
    idx_t temp = n;
    unsigned int digits[20];      /* 2^64 < 10^20, so 20 digits suffice */
    int len = 0;
    while (temp > 0)
    {
        digits[len++] = (unsigned int)(temp % 10);
        temp /= 10;
    }

    /* carryless convolution: c[i] = Σ d[k]*d[i-k] mod 10 */
    int maxlen = len * 2 - 1;
    unsigned int conv[40] = {0};  /* max 39 digits */
    for (int i = 0; i < maxlen; ++i)
    {
        unsigned int sum = 0;
        int kstart = (i < len) ? 0 : i - (len - 1);
        int kend   = (i < len) ? i : len - 1;
        for (int k = kstart; k <= kend; ++k)
            sum += digits[k] * digits[i - k];
        conv[i] = sum % 10;
    }

    /* rebuild the decimal number from most significant digit to LSB */
    seq_t res = 0;
    for (int i = maxlen - 1; i >= 0; --i)
        res = res * 10 + conv[i];

    return res;
}
