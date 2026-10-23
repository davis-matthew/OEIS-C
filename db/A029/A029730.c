/*
 * A029730  Numbers that are palindromic in base 16.
 * https://oeis.org/A029730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence of numbers that are palindromic in base 16. */
/* a(1) = 0, a(2) = 1, …  a(97) = 0x10101, etc. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute powers of 16 up to a safe bound.  For n ≈ 10 000
       the longest hex palindrome has at most 7 digits:
       0xFFFFFFF = 268 435 455.  16⁶ = 16 777 216, 16⁷ = 268 435 456. */
    idx_t pow16[8];
    pow16[0] = (idx_t)1;
    for (idx_t i = 1; i < 8; ++i)
        pow16[i] = pow16[i - 1] * (idx_t)16;

    /* Find the length of the palindrome that contains a(n). */
    idx_t cumulative = 0;
    idx_t length = 0;
    idx_t offsetWithinLength = 0;   /* 0‑based offset inside the block of a certain length */

    for (idx_t len = 1; ; ++len)
    {
        idx_t count;
        if (len == 1)
            count = 16;            /* digits 0‑15 */
        else
        {
            idx_t halfLenMinus1 = (len + 1) / 2 - 1;
            count = (idx_t)15 * pow16[(size_t)halfLenMinus1];
        }

        if (n <= cumulative + count)
        {
            length = len;
            offsetWithinLength = n - cumulative - 1;   /* 0‑based */
            break;
        }
        cumulative += count;
    }

    /* Single‑digit case: the value is the offset itself. */
    if (length == 1)
        return (seq_t)offsetWithinLength;

    /* Build the required palindrome. */
    idx_t halfLen = (length + 1) / 2;          /* ceil(length/2) */
    idx_t exp = halfLen - 1;                   /* number of free digits after the leading one */
    idx_t expPow = pow16[(size_t)exp];         /* 16^exp */

    idx_t offSeq = offsetWithinLength;
    idx_t firstDigit = (offSeq / expPow) + (idx_t)1;   /* 1 … 15 */
    idx_t restOffset = offSeq % expPow;

    unsigned char half[20];   /* holds the first half digits (most significant first) */
    half[0] = (unsigned char)firstDigit;

    /* Fill the remaining positions of the half (least significant to most). */
    idx_t temp = restOffset;
    for (int i = (int)halfLen - 1; i >= 1; --i)
    {
        half[i] = (unsigned char)(temp % (idx_t)16);
        temp /= (idx_t)16;
    }

    /* Assemble the full sequence of digits. */
    unsigned char digits[50];
    int d = 0;
    for (idx_t i = 0; i < halfLen; ++i)
        digits[d++] = half[i];

    if (length % 2 == 0)   /* even length: mirror all half digits */
    {
        for (int i = (int)halfLen - 1; i >= 0; --i)
            digits[d++] = half[i];
    }
    else                   /* odd length: mirror excluding the middle digit */
    {
        for (int i = (int)halfLen - 2; i >= 0; --i)
            digits[d++] = half[i];
    }

    /* Convert the digit array to the numeric value in base 16. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < d; ++i)
    {
        result = result * (seq_t)16 + (seq_t)digits[i];
    }

    return result;
}
