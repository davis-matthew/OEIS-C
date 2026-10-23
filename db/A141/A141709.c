/*
 * A141709  Least positive multiple of n which is palindromic in base 2,
 *          allowing for leading zeros (or: ignoring trailing zeros).
 * https://oeis.org/A141709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = least positive multiple of n which is palindromic in base
 *            2, allowing for leading zeros (equivalently: ignoring trailing
 *            zeros)
 * Range:     u8 n <= 18, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_binary_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_binary_palindrome.h"

seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;        /* cast index to sequence type once */
    /* iterate over odd multiples only – every even multiple
       has the same odd part when trailing zeros are removed. */
    for (idx_t t = 1; ; t += 2) {
        seq_t m = seq_n * (seq_t)t;   /* candidate multiple */

        /* strip trailing zeros (leading zeros in the reversed string) */
        seq_t k = m;
        while ((k & (seq_t)1) == 0) {
            k >>= 1;
        }

        if (is_binary_palindrome(k)) {
            return m;
        }
    }
}
