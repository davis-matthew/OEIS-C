/*
 * A140459  A positive integer n is included if n can be added to d(n) in
 *          binary without any carries, where d(n) is the number of positive
 *          divisors of n. In other words, no 1 in the binary representation
 *          of n is in the same position as any 1 in the binary representation
 *          of d(n).
 * https://oeis.org/A140459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) is the nth positive integer n such that, when written in binary,
 * n and d(n) (the number of divisors of n) share no 1‑bits in the same position.
 * Computed by straightforward brute‑force enumeration.
 */
#include <stdbool.h>

/*  Compute the divisor count of x using trial division.   */
static idx_t divisor_count(idx_t x)
{
    idx_t d = 1;
    idx_t tmp = x;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            d *= exp + 1;
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        d *= 2;
    return d;
}

/*  a(n) – nth term of sequence A140459.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety – offset is 1 */

    idx_t found = 0;
    idx_t cand = 1;

    while (found < n)
    {
        if ((cand & divisor_count(cand)) == 0)
            ++found;

        if (found == n) return (seq_t)cand;
        ++cand;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)cand;
}
