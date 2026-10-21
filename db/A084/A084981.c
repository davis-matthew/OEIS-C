/*
 * A084981  Numbers all of whose proper divisors are palindromic.
 * https://oeis.org/A084981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A084981: numbers all of whose proper divisors are palindromic.
   The function scans successive integers, counting those that satisfy the
   property until the requested index is reached.
   The search stops as soon as `n` terms have been found. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static bool divisors_palindromic(idx_t x)
{
    if (x == 1)                     /* 1 has no proper divisors */
        return true;

    for (idx_t d = 1; d * d <= x; ++d) {
        if (x % d == 0) {
            if (d < x && !is_palindrome((seq_t)d, 10))
                return false;
            idx_t e = x / d;
            if (e != d && e < x && !is_palindrome((seq_t)e, 10))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */

    idx_t found = 0;
    idx_t candidate = 1;

    while (found < n) {
        if (divisors_palindromic(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)(candidate);
        }
        ++candidate;
    }
    /* Should never reach here */
    return (seq_t)0;
}
