/*
 * A146525  a(n) is the largest nonnegative integer m such that 2*n*k-1 is
 *          prime for all k where 1<=k<=m. a(n) = 0 if 2n-1 is composite.
 * https://oeis.org/A146525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum m such that 2*n*k – 1 is prime for all k = 1
 *            .. m. If 2*n – 1 is composite, a(n) = 0. The implementation
 *            tests successive terms of the arithmetic progression until the
 *            first composite is found
 * Range:     u8 n <= 41, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Works for the numbers involved in this sequence (n ≤ 10 000). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2U) return false;
    if (x % 2U == 0U) return x == 2U;
    for (uint64_t d = 3U; d * d <= x; d += 2U)
        if (x % d == 0U)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  Guard against the pathological value 0, which
       cannot occur in the OEIS indexing but keeps the function safe
       when called with an unsigned wrap-around. */
    if (n == 0U)
        return 0U;

    /* Compute the first term 2*n*1 – 1.  If it is not prime, the
       sequence value is 0. */
    seq_t val = (seq_t)2U * (seq_t)n - 1U;
    if (!is_prime_uint64((uint64_t)val))
        return 0U;

    /* The progression begins with a prime.  Count it. */
    seq_t m = 1U;
    seq_t k = 2U;

    /* Repeatedly test 2*n*k – 1 until a composite appears. */
    while (true)
    {
        val = (seq_t)2U * (seq_t)n * k - 1U;
        if (is_prime_uint64((uint64_t)val))
        {
            ++m;
            ++k;
        }
        else
        {
            break;
        }
    }

    return m;
}
