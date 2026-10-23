/*
 * A130838  Sorted list of strings that can be obtained by starting with 12
 *          and repeatedly doubling any substring in place.
 * https://oeis.org/A130838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 if n = 1 a(n) = 1{1,2}^{n-2}2 for n ≥ 2
 *            (lexicographically sorted) The middle (n-2) digits correspond to
 *            the binary expansion of (n-2) with 1 for bit 0 and 2 for bit 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   _floor_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_log2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index. */
    if (n == 1) {
        return (seq_t)12;
    }

    /* For n ≥ 2:
       Let k = floor(log2(n)).
       Then length L = k + 2, and the offset inside this block is:
          offset = n - 2^k            (0‑based)
    */
    idx_t k = _floor_log2(n);
    idx_t start = ((idx_t)1) << k;
    idx_t offset = n - start;        /* 0 ≤ offset < 2^k */

    /* Build the decimal integer digit by digit. */
    seq_t res = 0;
    /* first digit '1' */
    res = res * (seq_t)10 + (seq_t)1;

    /* middle digits (k bits).   Process from most significant to least. */
    for (idx_t i = k; i > 0; --i) {
        seq_t digit = ((offset >> (i - 1)) & 1) ? (seq_t)2 : (seq_t)1;
        res = res * (seq_t)10 + digit;
    }

    /* last digit '2' */
    res = res * (seq_t)10 + (seq_t)2;

    return res;
}
