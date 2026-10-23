/*
 * A043044  Palindromes that start with 9.
 * https://oeis.org/A043044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindrome whose first digit is 9 (both odd and even
 *            length palindromes, in increasing order)
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  Work with seq_t for arbitrary precision. */
    seq_t idx = (seq_t)n;          /* current index in seq_t */

    for (idx_t k = 1; ; ++k) {
        /* count_k = 10^(k-1): number of palindromes for a given half-length k */
        seq_t count_k = pow10(k - 1);
        seq_t double_count = count_k * (seq_t)2;   /* 2 * count_k */

        if (idx <= double_count) {
            /* Determine whether the desired term has odd or even length. */
            bool odd = idx <= count_k;

            if (!odd)            /* even length case: subtract odd block */
                idx -= count_k;

            seq_t offset = idx - (seq_t)1;          /* 0‑based offset in this block */
            seq_t left_start = (seq_t)9 * pow10(k - 1);
            seq_t left_half = left_start + offset;

            if (odd) {
                /* odd length = 2*k-1 */
                seq_t prefix = left_half * pow10(k - 1);
                seq_t suffix = reverse(left_half / (seq_t)10);
                return prefix + suffix;
            } else {
                /* even length = 2*k */
                seq_t prefix = left_half * pow10(k);
                seq_t suffix = reverse(left_half);
                return prefix + suffix;
            }
        }

        /* Not found yet: skip this block and continue. */
        idx -= double_count;
    }
}
