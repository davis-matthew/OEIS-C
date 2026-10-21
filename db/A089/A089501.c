/*
 * A089501  Built from superfactorials A000178.
 * https://oeis.org/A089501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A089501, built from superfactorials.
   Formula: a(n) = sfac(n‑1) * sfac(2n+1) / sfac(n+1),  with
   sfac(m) = ∏_{k=1}^{m} k!.  For n = 0 this equals 1.
   Using the identity   sfac(m) = ∏_{i=1}^{m} i^{m-i+1},
   a(n) can be written as 
        a(n) = ∏_{i=1}^{n‑1} i^{2n‑i}
                · n^{n}
                · (n+1)^{n}
                · ∏_{i=n+2}^{2n+1} i^{2n‑i+2}.
   a(0) = 1.
   The program implements this product with binary exponentiation,
   which works for any integer or arbitrary‑precision type defined as
   seq_t.  It runs in O(n log n) time and uses only integer
   arithmetic.  The same code compiles in C11 and in C++17. 
*/

#include <stdint.h>          /* for uint64_t if needed */

static inline seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;

    /* lower part: i = 1 .. n-1, exponent = 2n - i */
    for (idx_t i = 1; i <= n - 1; ++i)
        res *= pow_int( (seq_t)i, 2 * n - i );

    /* middle terms: n^n and (n+1)^n */
    res *= pow_int( (seq_t)n, n );
    res *= pow_int( (seq_t)(n + 1), n );

    /* upper part: i = n+2 .. 2n+1, exponent = 2n - i + 2 */
    for (idx_t i = n + 2; i <= 2 * n + 1; ++i)
        res *= pow_int( (seq_t)i, 2 * n - i + 2 );

    return res;
}
