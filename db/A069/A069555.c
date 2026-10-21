/*
 * A069555  a(n) = smallest non-palindromic number k such that k and its digit
 *          reversal are divisible by n, or 0 if n is a multiple of 10.
 * https://oeis.org/A069555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the definition of A069555:
 *   a(n) = smallest non‑palindromic number k such that k and rev(k)
 *   are both divisible by n; a(n) = 0 if n is a multiple of 10.  The
 *   algorithm tests multiples of n in increasing order until the first
 *   non‑palindromic candidate whose digit reversal is also divisible
 *   by n is found.  It uses the helper functions reverse_decimal and
 *   is_palindrome from the utils directory. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* If n is a multiple of 10 the definition specifies 0. */
    if ((n % 10) == 0)
        return 0;

    const seq_t stride = (seq_t)n;      /* cast for seq_t arithmetic */
    seq_t m = 1;                        /* multiplier of n */

    for (;; ++m)
    {
        /* Candidate k = m * n */
        seq_t k = stride * m;

        /* Skip palindromic numbers. */
        if (is_palindrome(k, 10))
            continue;

        /* Reverse the decimal digits of k. */
        seq_t rev_k = reverse_decimal(k);

        /* Check divisibility of the reversed number by n. */
        if ((rev_k % stride) == 0)
            return k;
    }
}
