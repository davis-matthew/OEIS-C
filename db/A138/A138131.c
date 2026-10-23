/*
 * A138131  Palindromic cyclops numbers.
 * https://oeis.org/A138131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth decimal palindrome that contains exactly one digit
 *            zero
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 7381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */

    /* rank among palindromes with one zero, excluding the single-digit 0 */
    idx_t rank = n - 1;

    int m = 1;                            /* current half-length: L = 2*m+1 */
    seq_t pow9 = 9;                       /* 9^m */

    while (1) {
        seq_t cnt = pow9;                 /* number of palindromes for this m */
        if (rank <= (idx_t)cnt) {
            seq_t offset = (seq_t)(rank - 1);

            /* decode offset into base-9 digits (0..8), mapping to digits 1..9 */
            int left_digits[30] = {0};    /* enough for m up to 30 */
            seq_t rem = offset;
            for (int i = m - 1; i >= 0; --i) {
                left_digits[i] = (int)((rem % 9) + 1);
                rem /= 9;
            }

            /* construct the palindrome: left, 0, reversed left */
            seq_t val = 0;
            for (int i = 0; i < m; ++i)
                val = val * 10 + (seq_t)left_digits[i];

            val = val * 10;                /* center zero */

            for (int i = m - 1; i >= 0; --i)
                val = val * 10 + (seq_t)left_digits[i];

            return val;
        }

        /* move to next m */
        rank -= (idx_t)cnt;
        pow9 *= 9;                        /* next power of 9 */
        ++m;
    }
}
