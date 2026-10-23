/*
 * A043052  a(n)=(s(n)+2)/5, where s(n)=n-th base 5 palindrome that starts
 *          with 3.
 * https://oeis.org/A043052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+2)/5 where s(n) is the nth base‑5 palindrome
 *            beginning with digit 3 (in increasing numerical order). The
 *            sequence begins with a(1)=1, a(2)=4, a(3)=16, a(4)=17, …
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   count_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_pal.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for 0, return 0 */

    /* 1. Locate the length L of the palindrome corresponding to n. */
    idx_t remaining = n;
    idx_t L = 1;
    while (remaining > count_pal(L)) {
        remaining -= count_pal(L);
        ++L;
    }

    /* remaining now is the 1‑based index inside length‑L palindromes. */
    seq_t s;                    /* will hold s(n) */

    if (L == 1) {               /* only palindrome is "3" */
        s = 3;
        return (s + 2) / 5;     /* a(1)=1 */
    }

    /* 2. Determine the offset (0‑based) within the palindromes of this length. */
    idx_t offset = remaining - 1;
    idx_t m = (L - 1) / 2;     /* number of free digits (excluding the leading 3) */

    /* 3. Decode offset into the m free digits (base‑5, most significant first). */
    uint8_t var[64];           /* m will never exceed 63 for n up to very large values */
    idx_t tmp = offset;
    for (idx_t i = 0; i < m; ++i) {
        var[m - 1 - i] = tmp % 5;
        tmp /= 5;
    }

    /* 4. Construct the palindrome in base 5 and convert to decimal. */
    s = 3;                     /* leading digit */

    /* left side (including the middle for odd lengths). */
    for (idx_t i = 0; i < m; ++i)
        s = s * 5 + var[i];

    /* right side: mirror */
    if (L % 2 == 0) {          /* even length: mirror all m digits */
        for (idx_t i = m; i > 0; --i)
            s = s * 5 + var[i - 1];
    } else {                   /* odd length: mirror all but the middle */
        for (idx_t i = m - 1; i > 0; --i)
            s = s * 5 + var[i - 1];
    }

    /* trailing digit */
    s = s * 5 + 3;

    /* 5. Final formula. */
    return (s + 2) / 5;
}
