/*
 * A089717  Triangular numbers with palindromic indices.
 * https://oeis.org/A089717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = T( A002113(n) ) where T(m) = m*(m+1)/2 computed by first
 *            determining the nth base‑10 palindrome
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 866, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/rev_num.h"

static idx_t pow10_idx(int exp)
{
    idx_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 10;
    return r;
}

static seq_t make_palindrome(int len, int half_len, idx_t offset)
{
    idx_t start = pow10_idx(half_len - 1);          /* first half */
    idx_t left  = start + offset;                 /* left block */

    if (len % 2 == 0) {                           /* even length */
        idx_t pow_left = pow10_idx(half_len);
        seq_t rev = rev_num(left);
        return (seq_t)left * (seq_t)pow_left + rev;
    } else {                                       /* odd length */
        idx_t pow_left = pow10_idx(half_len - 1);
        seq_t rev = rev_num(left / 10);
        return (seq_t)left * (seq_t)pow_left + rev;
    }
}

static seq_t nth_palindrome(idx_t n)
{
    if (n <= 10)                /* 0 ≤ n-1 ≤ 9 */
        return (seq_t)(n - 1);

    idx_t remaining = n - 10;   /* skip the single‑digit values */

    for (int len = 2;; ++len)
    {
        int half_len = (len + 1) / 2;
        idx_t count  = 9 * pow10_idx(half_len - 1);   /* palindromes of this length */

        if (remaining <= count)                       /* desired length found */
        {
            idx_t offset = remaining - 1;             /* 0‑based offset */
            return make_palindrome(len, half_len, offset);
        }

        remaining -= count;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                    /* undefined for n < 1 */

    seq_t p   = nth_palindrome(n);                    /* nth palindromic number */
    seq_t res = p * (p + 1) / 2;                      /* triangular number */
    return res;
}
