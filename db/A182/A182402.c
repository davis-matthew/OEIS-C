/*
 * A182402  Total number of digits in n-th row of a triangle formed by the
 *          positive integers.
 * https://oeis.org/A182402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A058183(A000217(n)) - A058183(A000217(n-1))
 * Range:     u8 n <= 21, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8380 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Total number of digits in row n of the triangle of positive integers.
   The row n consists of the integers from T(n‑1)+1 to T(n) inclusive,
   where T(k)=k(k+1)/2 is the k‑th triangular number.  a(n) is just
   the sum of the decimal digit lengths of those integers.  The
   implementation uses only integer arithmetic and a few helper
   functions from the utils library.  The calculation is O(log10(T(n)))
   and therefore very fast for n up to at least 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Trivial case for safety: n=0 is not used in the sequence, but
       returning 0 avoids under‑flow when computing T(n-1).                */
    if (n == 0)
        return 0;

    /* T(k) = k(k+1)/2;  compute using seq_t to avoid overflow.          */
    seq_t low  = ((seq_t)(n - 1)) * ((seq_t)n)     / 2;   /* T(n-1)      */
    seq_t high = ((seq_t)n) * ((seq_t)(n + 1))     / 2;   /* T(n)        */
    seq_t start = low + 1;                         /* first number in the row */

    seq_t ans = 0;
    int d_start = num_digits(start);
    int d_end   = num_digits(high);

    for (int d = d_start; d <= d_end; ++d)
    {
        seq_t pow_dm1 = pow10(d - 1);          /* 10^(d-1)            */
        seq_t pow_d   = pow10(d);              /* 10^d                */

        seq_t first = start > pow_dm1 ? start : pow_dm1;
        seq_t last  = high < (pow_d - 1) ? high : (pow_d - 1);

        if (first > last)
            continue;

        seq_t count = last - first + 1;
        ans += count * (seq_t)d;
    }

    return ans;
}
