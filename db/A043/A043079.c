/*
 * A043079  (s(n)+1)/9, where s(n)=n-th base 9 palindrome that starts with 8.
 * https://oeis.org/A043079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/9 where s(n) is the n-th base‑9 palindrome *
 *            that starts with 8. The function enumerates these palindromes *
 *            in increasing numeric order
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t – the build supplies seq_t and idx_t */

static seq_t pow9(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 9;
    return r;
}

static seq_t count_palindromes_of_length(idx_t len)
{
    if (len == 1 || len == 2)
        return 1;
    if (len & 1)          /* odd length: 2k+1, k = (len-1)/2 */
        return pow9((len - 1) / 2);
    return pow9(len / 2 - 1);   /* even length: 2k, k = len/2 */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* index 0 is outside the sequence – behaviour undefined */
        return 0;

    idx_t idx   = n - 1;           /* zero‑based index */
    idx_t len   = 1;
    idx_t rem   = idx;

    /* find the length of the palindrome containing the n‑th element */
    while (1)
    {
        seq_t cnt = count_palindromes_of_length(len);
        if (rem < (idx_t)cnt)
            break;
        rem -= (idx_t)cnt;
        ++len;
    }

    /* special case: length 1 and 2 are trivial */
    if (len == 1)
    {
        seq_t val = 8;          /* "8" in base 9 */
        return (val + 1) / 9;   /* returns 1 */
    }
    if (len == 2)
    {
        seq_t val = 80;         /* "88" in base 9 */
        return (val + 1) / 9;   /* returns 9 */
    }

    /* For lengths >=3, construct the palindrome digits */
    int  inner_len;                /* number of free digits (excluding leading 8 and trailing 8) */
    if (len & 1)                   /* odd length */
        inner_len = (len - 1) / 2; /* includes the middle digit */
    else                           /* even length */
        inner_len = len / 2 - 1;

    /* convert rem to base‑9 with inner_len digits */
    uint8_t inner[32];            /* 32 > max needed for typical n */
    idx_t   offset = rem;
    for (int i = inner_len - 1; i >= 0; --i)
    {
        inner[i] = offset % 9;
        offset   /= 9;
    }

    /* build the full digit array */
    uint8_t full[32];
    int     pos = 0;
    full[pos++] = 8;             /* leading 8 */

    if (len & 1) /* odd length */
    {
        /* left side excluding middle */
        for (int i = 0; i < inner_len - 1; ++i)
            full[pos++] = inner[i];
        /* middle digit */
        full[pos++] = inner[inner_len - 1];
        /* right side, mirrored excluding trailing 8 */
        for (int i = inner_len - 2; i >= 0; --i)
            full[pos++] = inner[i];
    }
    else          /* even length */
    {
        /* left side excluding trailing 8 */
        for (int i = 0; i < inner_len; ++i)
            full[pos++] = inner[i];
        /* right side, mirrored */
        for (int i = inner_len - 1; i >= 0; --i)
            full[pos++] = inner[i];
    }

    full[pos++] = 8;             /* trailing 8 */

    /* compute the decimal value of the constructed base‑9 number */
    seq_t val = 0;
    for (int i = 0; i < pos; ++i)
        val = val * 9 + full[i];

    return (val + 1) / 9;
}
