/*
 * A043065  a(n)=(s(n)+7)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 1.
 * https://oeis.org/A043065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+7)/8, where s(n) is the n‑th base‑8 palindrome that
 *            starts with 1
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow8idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow8idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not part of the sequence */

    /* --- Find palindrome length L ------------------------------- */
    idx_t L   = 1;
    idx_t rem = n;                /* remaining index within this length */

    for (;;)
    {
        idx_t free = (L <= 2) ? 0 : ((L % 2 == 0) ? (L / 2 - 1) : (L / 2));
        idx_t cnt  = pow8idx(free);                /* number of palindromes of this length */

        if (rem <= cnt) break;                    /* desired length found */
        rem -= cnt;
        ++L;
    }

    /* --- Build the first half of the palindrome ----------------- */
    idx_t firstHalfLen = (L + 1) / 2;              /* ceil(L/2) */
    uint8_t firstHalf[64] = {0};
    uint8_t full[64]     = {0};

    firstHalf[0] = 1;                              /* leading digit is always 1 */

    idx_t tmp = rem - 1;                           /* 0‑based index within this length */
    for (idx_t i = firstHalfLen - 1; i > 0; --i)    /* fill the free digits */
    {
        firstHalf[i] = (uint8_t)(tmp % 8);
        tmp /= 8;
    }

    /* --- Mirror the first half to get the full palindrome ------- */
    for (idx_t i = 0; i < firstHalfLen; ++i)
        full[i] = firstHalf[i];

    if (L % 2 == 0)                                 /* even length */
    {
        for (idx_t i = 0; i < firstHalfLen; ++i)
            full[L - 1 - i] = firstHalf[i];
    }
    else                                            /* odd length */
    {
        for (idx_t i = 0; i < firstHalfLen - 1; ++i)
            full[L - 1 - i] = firstHalf[i];
    }

    /* --- Convert the base‑8 digits to a decimal value ---------- */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * (seq_t)8 + full[i];

    /* --- Final formula ------------------------------------------ */
    return (val + 7) / 8;
}
