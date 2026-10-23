/*
 * A043262  Sum of digits of n-th base 3 palindrome.
 * https://oeis.org/A043262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of n-th base-3 palindrome
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n <= 3 (length 1 palindromes). */
    if (n == 0) return 0;          /* behavior below offset is irrelevant */
    if (n <= 3) return (seq_t)(n - 1);  /* 0, 1, 2 */

    /* Find the length L of the n-th palindrome. */
    idx_t rem = n - 3;  /* skip the three length-1 palindromes */
    idx_t L = 2;
    while (1) {
        idx_t count;
        if (L % 2 == 0) {          /* even length */
            idx_t k = L / 2;
            count = 2;              /* first digit can be 1 or 2 */
            for (idx_t i = 0; i < k - 1; ++i) count *= 3;  /* 3^(k-1) */
        } else {                    /* odd length (L >= 3) */
            idx_t k = (L - 1) / 2;
            count = 2;              /* first digit can be 1 or 2 */
            for (idx_t i = 0; i < k; ++i) count *= 3;      /* 3^k */
        }
        if (rem <= count) break;
        rem -= count;
        ++L;
    }

    /* rem now is 1-based index within the block of length L. */
    idx_t idx0 = rem - 1;  /* 0-based */

    if (L == 1) {
        return (seq_t)idx0;          /* 0, 1, 2 */
    }

    /* Determine the first half digits. */
    idx_t m = (L + 1) / 2;          /* ceil(L/2) */
    idx_t blockSize = 1;
    for (idx_t i = 0; i < m - 1; ++i) blockSize *= 3;  /* 3^(m-1) */

    unsigned char digits[32];        /* enough for any reasonable length */
    digits[0] = (unsigned char)((idx0 < blockSize) ? 1 : 2);
    if (idx0 >= blockSize) idx0 -= blockSize;

    /* Fill the remaining (m-1) digits (in base 3). */
    for (int pos = (int)m - 1; pos >= 1; --pos) {
        digits[pos] = (unsigned char)(idx0 % 3);
        idx0 /= 3;
    }

    /* Compute the digit sum. */
    seq_t sum_half = digits[0];
    for (idx_t pos = 1; pos < m; ++pos) sum_half += digits[pos];

    seq_t middle = digits[m - 1];
    seq_t result;
    if ((L & 1) == 0) {                           /* even length */
        result = 2 * sum_half;
    } else {                                       /* odd length */
        result = 2 * (sum_half - middle) + middle;
    }

    return result;
}
