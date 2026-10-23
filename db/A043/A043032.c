/*
 * A043032  Base-9 palindromes that start with 5.
 * https://oeis.org/A043032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base‑9 palindrome whose most significant digit
 *            is 5. The sequence is generated directly from the position n by
 *            combinatorial counting rather than by exhaustive search
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to zero‑based index */
    idx_t rem = n - 1;

    /* Find the half‑length k such that
       the first 2*9^(k‑1) palindromes belong to lengths 2k‑1 and 2k. */
    idx_t pow9 = 1;   /* 9^(k‑1) */
    idx_t k = 1;      /* current k (half‑length) */

    while (rem >= pow9 * 2) {
        rem -= pow9 * 2;
        pow9 *= 9;
        ++k;
    }

    /* Determine whether we are in the odd‑length or even‑length block */
    bool odd = rem < pow9;
    idx_t v = odd ? rem : rem - pow9;   /* index within the block (0‑based) */

    /* Build the left half of the palindrome, digits[0] is the most
       significant digit. */
    uint8_t digits[64];      /* enough for all expected k */
    digits[0] = 5;           /* first digit is fixed to 5 */

    idx_t tmp = v;
    for (int i = (int)k - 1; i >= 1; --i) {
        digits[i] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Compute the decimal value of the full palindrome in base 9 */
    seq_t val = 0;

    /* Left half */
    for (int i = 0; i < (int)k; ++i)
        val = val * 9 + digits[i];

    /* Right half */
    if (odd) {
        for (int i = (int)k - 2; i >= 0; --i)
            val = val * 9 + digits[i];
    } else {
        for (int i = (int)k - 1; i >= 0; --i)
            val = val * 9 + digits[i];
    }

    return val;
}
