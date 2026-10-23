/*
 * A115745  Pentagonal numbers (A000326) that are the product of 2 palindromes
 *          greater than 1.
 * https://oeis.org/A115745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth pentagonal number P(m) = m*(3*m-1)/2 that can be
 *            expressed as a product of two palindromes greater than 1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Previously the code used the triangular-number formula, yielding wrong
   terms.  The correct formula above gives the required sequence. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"   /* is_palindrome(seq_t, idx_t) */
#include "../utils/floor_sqrt.h"      /* floor_sqrt(seq_t) */

/* Return true iff the value X can be written as a product of two
   decimal palindromes each greater than 1.  X is treated as a
   seq_t.  For the range of numbers encountered (< 2^32) a 64‑bit
   representation is sufficient for divisor enumeration. */
static bool has_palindrome_product(seq_t X)
{
    if (X <= 1)
        return false;

    /* Using 64‑bit arithmetic for divisor search; the values are small. */
    uint64_t xu = (uint64_t)X;
    uint64_t lim = (uint64_t)floor_sqrt(X);

    for (uint64_t d = 2; d <= lim; ++d) {
        if (xu % d == 0) {
            uint64_t other = xu / d;
            if (other <= 1)
                continue;
            if (is_palindrome((seq_t)d, 10) &&
                is_palindrome((seq_t)other, 10))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;          /* pentagonal index */

    while (count < n) {
        /* Pentagonal number: P(m) = m*(3*m-1)/2 */
        seq_t pent = ((seq_t)m * ((seq_t)3 * m - 1)) / 2;
        if (has_palindrome_product(pent)) {
            ++count;
            if (count == n)
                return pent;
        }
        ++m;
    }
    return 0;   /* unreachable, satisfies the compiler */
}
