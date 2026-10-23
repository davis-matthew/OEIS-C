/*
 * A154809  Numbers whose binary expansion is not palindromic.
 * https://oeis.org/A154809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the nth positive integer whose binary expansion is not a
 *            palindrome. Computes this by a simple linear scan, which is O(n)
 *            and fast enough for n up to about 10000
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Test whether a seq_t value has a palindromic binary representation. */
static bool is_bin_palindrome(seq_t x)
{
    /* 0 and 1 are palindromes. */
    unsigned len = bitlen_seq_t(x);
    seq_t one = 1;          /* use a seq_t instance for bit masking */

    for (unsigned i = 0; i < len / 2; ++i) {
        seq_t lbit = (x >> i) & one;
        seq_t rbit = (x >> (len - 1 - i)) & one;
        if (lbit != rbit)
            return false;
    }
    return true;
}

/* Return the nth number (n >= 1) whose binary representation is not a palindrome. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur   = 1;  /* start from 1; 1 itself is a palindrome, so it will be skipped */

    while (true) {
        if (!is_bin_palindrome(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        cur += 1;
    }
}
