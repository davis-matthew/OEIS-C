/*
 * A062916  Non-palindromic number and its reversal are both multiples of 19.
 * https://oeis.org/A062916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Non‑palindromic numbers divisible by 19 whose reversal is also
 *            divisible by 19 Computation: brute‑force search over natural
 *            numbers. (a(n) = smallest integer x such that x ≡ 0 (mod 19),
 *            rev(x) ≡ 0 (mod 19), rev(x) ≠ x, and there are n-1 such numbers
 *            less than x)
 * Range:     u8 none, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_uint64.h"

seq_t a(idx_t n)
{
    idx_t target = n;               /* number of terms we need to find */
    idx_t found  = 0;               /* how many terms we have found so far */
    idx_t x      = 1;               /* current integer being tested */
    while (true) {
        if (x % 19 == 0) {
            seq_t rev = (seq_t)reverse_uint64((uint64_t)x);
            if (rev % 19 == 0 && rev != (seq_t)x) {
                found++;
                if (found == target)
                    return (seq_t)x;
            }
        }
        x++;                         /* next integer to test */
    }
}
