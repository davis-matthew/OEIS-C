/*
 * A086879  Number of symmetric singular n X n matrices over GF(2).
 * https://oeis.org/A086879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n+1)/2) - A086812(n) where A086812(n) is the number
 *            of nonsingular symmetric n×n matrices over GF(2). A086812(n)
 *            satisfies the recurrence: A086812(1) = 1 For n > 1: if n is
 *            even: A086812(n) = 2^n * A086812(n-1) if n is odd: A086812(n) =
 *            (2^n - 1) * A086812(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper functions pow2(idx_t) and seq_mul(seq_t, seq_t)
   which are available in the build environment.  It is O(n) time and works for any
   integer types chosen for seq_t and idx_t. */

#include <stdint.h>   /* for uint64_t, etc. */
#include <stdbool.h>  /* just in case */

#include "../utils/pow2.h"  /* provides seq_t pow2(idx_t) */
#include "../utils/seq_mul.h"  /* provides seq_t seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Handle the minimal case directly (offset starts at 1). */
    if (n == 0)
        return 0; /* undefined behavior for n < 1 per the offset spec */

    /* Compute B = A086812(n), the number of nonsingular symmetric matrices. */
    seq_t B = 1; /* seq_t is unary 1 convertible */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t pow_i = pow2(i);          /* 2^i   */
        if ((i & 1) == 0)               /* even i */
        {
            B = seq_mul(B, pow_i);      /* B *= 2^i */
        }
        else                            /* odd i */
        {
            seq_t pow_i_minus1 = pow_i - 1;  /* 2^i - 1 */
            B = seq_mul(B, pow_i_minus1);    /* B *= (2^i - 1) */
        }
    }

    /* Compute total = 2^(n*(n+1)/2) */
    seq_t nn = (seq_t)n;
    seq_t exp = nn * (nn + 1) / 2;    /* intermediate in seq_t to avoid overflow */
    seq_t total = pow2((idx_t)exp);

    /* a(n) = total - B */
    return total - B;
}
