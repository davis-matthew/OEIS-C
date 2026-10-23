/*
 * A043061  (s(n)+4)/7, where s(n)=n-th base 7 palindrome that starts with 3.
 * https://oeis.org/A043061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+4)/7 where s(n) is the n-th base-7 palindrome
 *            starting with 3
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow7idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow7idx.h"

seq_t a(idx_t n) {
    /* find the length L of the n‑th palindrome */
    idx_t cum = 0;
    uint32_t L = 1;
    while (1) {
        idx_t count;
        if (L % 2 == 0) {                 /* even length 2k */
            uint32_t k = L / 2;
            count = pow7idx(k - 1);
        } else {                          /* odd length 2k+1 */
            uint32_t k = (L - 1) / 2;
            count = pow7idx(k);
        }
        if (cum + count >= n)
            break;
        cum += count;
        ++L;
    }

    /* rank within this length, 1‑based */
    idx_t r = n - cum;

    /* digits of the palindrome (base 7) */
    uint8_t digits[128] = {0};
    digits[0] = 3;                          /* leading digit */

    /* number of free digits */
    uint32_t k, freeCount;
    if (L % 2 == 0) {                       /* even length */
        k = L / 2;
        freeCount = k - 1;
    } else {                                /* odd length */
        k = (L - 1) / 2;
        freeCount = k;
    }

    /* extract free digits from r-1 in base 7 (most significant first) */
    uint8_t freeDigits[128] = {0};
    idx_t val = r - 1;
    for (int i = (int)freeCount - 1; i >= 0; --i) {
        freeDigits[i] = (uint8_t)(val % 7);
        val /= 7;
    }

    /* fill the free digits into the palindrome */
    if (L % 2 == 0) {                      /* even length 2k */
        for (uint32_t i = 1; i < k; ++i)
            digits[i] = freeDigits[i - 1];
    } else {                               /* odd length 2k+1 */
        for (uint32_t i = 1; i < k; ++i)
            digits[i] = freeDigits[i - 1];
        if (freeCount > 0)
            digits[k] = freeDigits[freeCount - 1];   /* centre digit */
    }

    /* mirror to the right side */
    for (uint32_t i = 0; i < L / 2; ++i)
        digits[L - 1 - i] = digits[i];

    /* convert from base 7 to decimal */
    seq_t s = 0;
    for (uint32_t i = 0; i < L; ++i)
        s = s * 7 + digits[i];

    /* compute a(n) */
    return (s + 4) / 7;
}
