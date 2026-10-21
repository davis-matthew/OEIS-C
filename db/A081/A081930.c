/*
 * A081930  Triangle read by rows in which row n gives n smallest palindromic
 *          numbers of n digits each.
 * https://oeis.org/A081930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = nth term of the triangle read by rows where row r
 *            contains the r smallest palindromes of r digits. The function
 *            works for any integer width defined by seq_t and idx_t. It uses
 *            only integer arithmetic
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow10_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_exponent.h"

static seq_t rev_digits(seq_t v)
{
    seq_t r = 0;
    while (v > 0) {
        r = r * 10 + (v % 10);
        v /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Row 1 is a special case: 0 is the smallest 1‑digit palindrome */
    if (n == 1)
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n;          /* high = n is always sufficient */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms before this row */
    unsigned __int128 sumPrev = (unsigned __int128)(r - 1) * r / 2;
    idx_t prevCount = (idx_t)sumPrev;

    /* Position k (0‑based) within row r */
    idx_t k = n - prevCount - 1;       /* 0 <= k < r */

    /* Length of the left half (ceil(r/2)) */
    idx_t half = (r + 1) / 2;

    /* Smallest left half: 10^(half-1) */
    seq_t start = pow10_exponent(half - 1);

    /* k-th left half */
    seq_t left = start + (seq_t)k;

    seq_t palindrome;
    if (r % 2 == 0) {                 /* even length */
        seq_t rev = rev_digits(left);
        seq_t powHalf = pow10_exponent(half);
        palindrome = left * powHalf + rev;
    } else {                           /* odd length */
        seq_t rev = rev_digits(left / 10);
        seq_t powHalfMinus1 = pow10_exponent(half - 1);
        palindrome = left * powHalfMinus1 + rev;
    }
    return palindrome;
}
