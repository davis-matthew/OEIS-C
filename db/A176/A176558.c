/*
 * A176558  a(n) is the reverse concatenation of divisors of n.
 * https://oeis.org/A176558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the reverse concatenation of the divisors of n. That is,
 *            write the divisors of n in decreasing order and concatenate them
 *            as decimal digits. Example: n = 12, divisors = 1,2,3,4,6,12 →
 *            a(12) = 1264321
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 29, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   digit_len_generic, pow10_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_len_generic.h"
#include "../utils/pow10_generic.h"

/* -----------------------------------------------------------------------
   Helper: integer square root for uint64_t (floor sqrt).
   Uses binary search with 128‑bit multiplication to avoid overflow.
   ----------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, sq;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        /* compute mid*mid without overflow */
        unsigned __int128 prod = (unsigned __int128)mid * (unsigned __int128)mid;
        if (prod == n) {
            return mid;
        } else if (prod < (unsigned __int128)n) {
            low = mid + 1;
            sq = mid;
        } else {
            high = mid - 1;
        }
    }
    return (low > 0) ? low - 1 : 0;
}

/* -----------------------------------------------------------------------
   Main sequence function
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n==0 return 0. */
    if (n == 0) return 0;

    seq_t result = 0;
    idx_t sq = isqrt_u64(n);          /* floor(sqrt(n)) */

    /* First pass: larger divisors (n/i) for i = 1 … sqrt(n) */
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            seq_t d = (seq_t)(n / i);    /* larger divisor */
            int len = digit_len_generic(d);
            result = result * pow10_generic(len) + d;
        }
    }

    /* Second pass: smaller divisors (i) in decreasing order.
       Skip the duplicate divisor when n is a perfect square. */
    for (idx_t i = sq; i > 0; --i) {
        if (n % i == 0) {
            if ((n / i) == i) continue; /* skip duplicate square root */
            seq_t d = (seq_t)i;          /* smaller divisor */
            int len = digit_len_generic(d);
            result = result * pow10_generic(len) + d;
        }
    }

    return result;
}
