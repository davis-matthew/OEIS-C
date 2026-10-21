/*
 * A082272  Smallest palindrome m such that n + m (m>0) is also a palindrome,
 *          or 0 if no such number exists.
 * https://oeis.org/A082272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082272: a(n) = smallest palindrome m > 0 such that n + m is also a palindrome;
   returns 0 if no such m exists up to a practical bound. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    const idx_t MAX_M = 1000000ULL;          /* practical search upper bound      */
    idx_t m = 1;

    while (m <= MAX_M) {
        seq_t mseq = (seq_t)m;
        if (is_palindrome(mseq, 10) && is_palindrome(nseq + mseq, 10))
            return mseq;                     /* found the smallest admissible m  */
        m += 1;
    }
    return (seq_t)0;                         /* no admissible m within bound     */
}
