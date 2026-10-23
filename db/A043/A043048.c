/*
 * A043048  a(n)=(s(n)+2)/4, where s(n)=n-th base 4 palindrome that starts
 *          with 2.
 * https://oeis.org/A043048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (s(n)+2)/4
 * where s(n) is the n-th base‑4 palindrome that starts with digit 2.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 4 in an idx_t array (sufficient for n up to 10 000). */
    static idx_t pow4[21];
    static int pow4_ready = 0;
    if (!pow4_ready) {
        pow4[0] = 1;
        for (int i = 1; i < 21; ++i)
            pow4[i] = pow4[i - 1] * 4;
        pow4_ready = 1;
    }

    /* Find the length L of the n-th palindrome. */
    idx_t cumulative = 0;
    int L = 0;
    while (1) {
        ++L;
        idx_t count = pow4[(L - 1) / 2];   /* 4^k palindromes of this length */
        if (cumulative + count >= n)
            break;
        cumulative += count;
    }

    idx_t inner = n - cumulative - 1;                    /* 0‑based index within length L */
    int k = (L - 1) / 2;                                 /* free digits after the leading 2 */

    unsigned char digits[21];                            /* enough for L ≤ 15 when n ≤ 10 000 */
    digits[0] = 2;                                      /* most significant digit fixed to 2 */

    /* Fill the free digits (positions 1 … k). */
    for (int i = 0; i < k; ++i) {
        unsigned char d = (unsigned char)((inner / pow4[k - 1 - i]) % 4);
        digits[1 + i] = d;
    }

    /* Mirror to the right half, including the last digit. */
    for (int i = 0; i < L / 2; ++i)
        digits[L - 1 - i] = digits[i];

    /* Convert digit array to its decimal value s. */
    seq_t s = 0;
    for (int i = 0; i < L; ++i)
        s = s * 4 + (seq_t)digits[i];

    /* Result is (s+2)/4.  Division is exact because s ends with digit 2. */
    return (s + 2) / 4;
}
