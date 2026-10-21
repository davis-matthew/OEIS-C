/*
 * A084019  a(n) = 9's complement of n-th palindrome (A002113).
 * https://oeis.org/A084019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 9's complement of the (n-1)-th palindrome (A002113 with 0
 *            as the first term)
 * Range:     u8 n <= 19, u16 n <= 199, u32 n <= 9348, u64 n <= 9348,
 *            u128 n <= 9348, big any n
 * Helpers:   pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10_seq.h"

seq_t a(idx_t n)
{
    /* n is 1‑indexed; first palindrome used is 0 */
    idx_t m = n - 1;              /* zero‑based index into the palindrome list */

    idx_t total = 0;
    idx_t L = 1;                   /* current palindrome length */

    while (1) {
        idx_t count;
        if (L == 1) {
            count = 10;                 /* 0 … 9 */
        } else {
            idx_t half = (L + 1) / 2;   /* ceil(L/2) */
            count = 9;
            for (idx_t i = 1; i < half; ++i)
                count *= 10;
        }

        if (m >= total + count) {
            total += count;
            ++L;
        } else {
            break;
        }
    }

    /* offset within palindromes of length L */
    idx_t offset = m - total;          /* 0‑based */

    seq_t palindrome;

    if (L == 1) {
        palindrome = offset;           /* single‑digit palindromes: 0 … 9 */
    } else {
        idx_t half = (L + 1) / 2;
        seq_t base = 1;
        for (idx_t i = 1; i < half; ++i)
            base *= 10;                 /* 10^(half-1) */

        seq_t prefix = base + offset;   /* first half of the palindrome */
        palindrome = prefix;

        seq_t tmp = prefix;
        if (L & 1)                     /* odd length – drop the middle digit */
            tmp /= 10;

        while (tmp > 0) {
            palindrome = palindrome * 10 + (tmp % 10);
            tmp /= 10;
        }
    }

    seq_t nines = pow10_seq(L);        /* 10^L   */
    seq_t result = nines - 1 - palindrome; /* 9's complement */

    return result;
}
