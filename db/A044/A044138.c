/*
 * A044138  Numbers n such that string 0,0 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 49 * k where k is the nth positive integer such that
 *            (k-1) * is base‑7 without any consecutive zeros. Implemented by
 *            brute‑force search
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_consecutive_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_consecutive_zero.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; defensive for n==0 */

    idx_t count = 0;
    for (idx_t k = 1;; ++k) {
        if (!has_consecutive_zero((seq_t)(k - 1))) {
            ++count;
            if (count == n)
                return (seq_t)49 * (seq_t)k;
        }
    }
}
