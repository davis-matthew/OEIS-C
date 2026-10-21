/*
 * A211100  Number of factors in Lyndon factorization of binary expansion of
 *          n.
 * https://oeis.org/A211100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the number of factors in the Lyndon factorization
 * of the binary representation of n using Duval’s algorithm. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 the binary string is "0", giving one factor. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build the binary string of n (most significant bit first). */
    /* Maximum possible length for 64‑bit n is 64, so a 65‑byte buffer is safe. */
    char s[65];
    size_t len = 0;
    idx_t temp = n;
    while (temp > 0) {
        s[len++] = ((temp & (idx_t)1) ? '1' : '0');
        temp >>= 1;
    }
    /* The bits were collected LSB first; reverse them. */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

    /* Duval's algorithm to count the Lyndon factors. */
    size_t i = 0;
    seq_t count = 0;
    while (i < len) {
        size_t j = i + 1;
        size_t k = i;
        while (j < len && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                k = i;
            } else {
                ++k;
            }
            ++j;
        }
        size_t l = j - k;               /* length of the Lyndon word found */
        while (i <= k) {                /* repeat this factor as needed */
            ++count;
            i += l;
        }
    }
    return count;
}
