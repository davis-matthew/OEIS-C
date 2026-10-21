/*
 * A069289  Sum of odd divisors of n <= sqrt(n).
 * https://oeis.org/A069289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑{ d | d divides n, d odd, d ≤ ⌊√n⌋ } d
 * Range:     u8 n <= 4094, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for 64‑bit unsigned integers.  */
static idx_t isqrt_u64(idx_t n)
{
    /* Uses the integer square root algorithm of Knuth vol. 4A §4.1.2.  */
    idx_t result = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* 2^62 ≤ max uint64_t square root */

    /* Find the highest power of four ≤ n. */
    while (bit > n) bit >>= 2;

    /* Iterate while bit is non‑zero. */
    while (bit)
    {
        if (n >= result + bit)
        {
            n -= result + bit;
            result = (result >> 1) + bit;
        }
        else
        {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Remove trailing zeros: odd divisors only.  */
    idx_t s = isqrt_u64(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= s; d += 2)
    {
        if (n % d == 0)
            sum += (seq_t)d;
    }
    return sum;
}
