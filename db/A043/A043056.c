/*
 * A043056  a(n)=(s(n)+3)/6, where s(n)=n-th base 6 palindrome that starts
 *          with 3.
 * https://oeis.org/A043056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/6 where s(n) is the n‑th base‑6 palindrome
 *            starting with 3
 * Range:     u8 n <= 8, u16 n <= 89, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t pow6(idx_t e)
{
    idx_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 6;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for out‑of‑range checks */
        return 0;

    /* Find the length L of the n‑th palindrome */
    idx_t remaining = n - 1;   /* zero‑based index among all palindromes */
    idx_t L = 1;
    while (1)
    {
        /* number of free digits = floor((L-1)/2) */
        idx_t freeCnt = (L / 2) - (L % 2 == 0 ? 1 : 0);
        idx_t count = pow6(freeCnt);
        if (remaining < count)
            break;
        remaining -= count;
        ++L;
    }

    /* Build the palindrome digits */
    uint8_t d[32] = {0};
    d[0] = 3;                 /* leading digit is always 3 */

    idx_t freeCnt = (L / 2) - (L % 2 == 0 ? 1 : 0);
    uint8_t freeDigits[32] = {0};
    idx_t tmp = remaining;
    for (idx_t i = 0; i < freeCnt; ++i)          /* least‑significant base‑6 digit first */
    {
        freeDigits[freeCnt - 1 - i] = tmp % 6;
        tmp /= 6;
    }

    idx_t k = L / 2;   /* half length for mirroring */
    if (L % 2 == 0)
    {
        /* even length: only left side free digits (positions 1 .. k-1) */
        for (idx_t i = 0; i < freeCnt; ++i)
            d[1 + i] = freeDigits[i];
    }
    else
    {
        /* odd length: middle digit at position k */
        if (freeCnt > 0)
            d[k] = freeDigits[freeCnt - 1];
        /* remaining left side free digits */
        for (idx_t i = 0; i + 1 < freeCnt; ++i)
            d[1 + i] = freeDigits[i];
    }

    /* Mirror left side onto right side */
    for (idx_t i = 0; i < k; ++i)
        d[L - 1 - i] = d[i];

    /* Convert base‑6 digits to decimal value */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * (seq_t)6 + (seq_t)d[i];

    /* Compute a(n) = (s(n)+3)/6 */
    seq_t result = (val + (seq_t)3) / (seq_t)6;
    return result;
}
