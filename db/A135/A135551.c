/*
 * A135551  Number of bases b, 1 < b < n, in which n is a palindrome.
 * https://oeis.org/A135551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8801, u16 n <= 8801, u32 n <= 8801, u64 n <= 8801,
 *            u128 n <= 8801, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A135551 – Number of bases b, 1 < b < n, in which n is a palindrome.
 * For n <= 2 the value is 0.
 *
 * Algorithm: for every base b from 2 to n‑1 convert n to that base and
 * test whether the resulting digit sequence is a palindrome.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t cnt = 0;
    for (idx_t b = 2; b < n; ++b) {
        idx_t t = n;
        uint16_t digits[64];
        int len = 0;
        while (t > 0) {
            digits[len++] = (uint16_t)(t % b);
            t /= b;
        }
        bool pal = true;
        for (int i = 0, j = len - 1; i < j; ++i, --j) {
            if (digits[i] != digits[j]) {
                pal = false;
                break;
            }
        }
        if (pal)
            ++cnt;
    }
    return cnt;
}
