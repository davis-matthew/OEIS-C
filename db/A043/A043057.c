/*
 * A043057  a(n)=(s(n)+2)/6, where s(n)=n-th base 6 palindrome that starts
 *          with 4.
 * https://oeis.org/A043057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n) + 2) / 6, where s(n) is the n‑th base‑6 palindrome
 *            that starts with digit 4
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t count_len(int len)
{
    if (len == 1) return 1;
    int k = len / 2;          /* floor(len/2) */
    idx_t cnt = 1;
    if (len % 2 == 0) {       /* even length: free = k-1 */
        for (int i = 1; i < k; ++i) cnt *= 6;
    } else {                  /* odd length: free = k-1, middle digit gives *6 */
        cnt = 6;
        for (int i = 1; i < k; ++i) cnt *= 6;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find the length of the desired palindrome */
    idx_t rem = n;
    int len = 1;
    while (1) {
        idx_t cnt = count_len(len);
        if (rem <= cnt) break;
        rem -= cnt;
        ++len;
    }

    /* Index within this length (0‑based) */
    idx_t idx0 = rem - 1;

    /* Build the base‑6 digits of the palindrome */
    int k = len / 2;                     /* floor(len/2) */
    uint8_t digits[64] = {0};             /* enough for all required lengths */
    digits[0] = 4;                        /* most significant digit fixed to 4 */

    if (len == 1) {
        /* nothing else to do */
    } else if (len % 2 == 0) {            /* even length */
        idx_t temp = idx0;
        for (int pos = k - 1; pos >= 1; --pos) {
            digits[pos] = (uint8_t)(temp % 6);
            temp /= 6;
        }
        for (int i = 0; i < k; ++i)
            digits[len - 1 - i] = digits[i];
    } else {                              /* odd length */
        idx_t midDigit = idx0 % 6;
        idx_t leftIndex = idx0 / 6;
        for (int pos = k - 1; pos >= 1; --pos) {
            digits[pos] = (uint8_t)(leftIndex % 6);
            leftIndex /= 6;
        }
        digits[k] = (uint8_t)midDigit;
        for (int i = 0; i < k; ++i)
            digits[len - 1 - i] = digits[i];
    }

    /* Convert the base‑6 digits to a decimal value */
    seq_t val = 0;
    for (int i = 0; i < len; ++i)
        val = val * 6 + digits[i];

    /* Apply the sequence definition: (s + 2) / 6 */
    val = (val + 2) / 6;
    return val;
}
