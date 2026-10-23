/*
 * A157102  Tuple-chromatic Van der Waerden numbers.
 * https://oeis.org/A157102
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * (smallest prime factor of n) + 1
 * Range:     u8 n <= 16, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the smallest prime factor of a positive integer n (n >= 2). */
static uint64_t spf_uint64(uint64_t n)
{
    if (n <= 2ULL) return n;           /* spf(2) = 2 */
    if (n % 2ULL == 0ULL) return 2ULL; /* even numbers */

    /* test odd divisors up to sqrt(n) */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
    {
        if (n % d == 0ULL) return d;
    }
    /* n is prime */
    return n;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* values below the offset are undefined */
    uint64_t spf = spf_uint64((uint64_t)n);
    seq_t res = (seq_t)(n - 1) * (seq_t)spf + (seq_t)1;
    return res;
}
