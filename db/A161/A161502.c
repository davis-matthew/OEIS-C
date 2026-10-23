/*
 * A161502  a(n) is the smallest number of binary digits that when appended to
 *          the right side of the binary representation of n, forms a binary
 *          palindrome.
 * https://oeis.org/A161502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest k such that the suffix of the binary
 *            representation of n starting at bit k is a palindrome
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* For n >= 1. */
    /* Find bit length L. */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        L++;
        tmp >>= 1;
    }
    if (L == 0) /* n == 0, not used for this sequence */
        return (seq_t)0;

    /* Extract bits into an array msb->lsb. */
    unsigned char bits[64];           /* L <= 64 because n is uint64_t */
    for (idx_t i = 0; i < L; ++i)
        bits[i] = (unsigned char)((n >> (L - 1 - i)) & 1ULL);

    /* Find minimal k (0 <= k <= L) such that bits[k..L-1] is a palindrome. */
    for (idx_t k = 0; k <= L; ++k) {
        bool ok = true;
        idx_t left = k, right = L - 1;
        while (left < right) {
            if (bits[left] != bits[right]) {
                ok = false;
                break;
            }
            left++;
            right--;
        }
        if (ok)
            return (seq_t)k;
    }

    /* In practice we always return within the loop. */
    return (seq_t)L;
}
