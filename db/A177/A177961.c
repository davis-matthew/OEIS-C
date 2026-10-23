/*
 * A177961  a(1)=2. Otherwise the average of the smallest prime divisors of
 *          2n-1 and 2n+1.
 * https://oeis.org/A177961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A090368(n)+A090368(n+1))/2 * where A090368(k) is the
 *            smallest prime divisor of the odd integer 2k-1. * Therefore
 *            a(1)=2, and for n>1 a(n) equals the average of the * smallest
 *            prime divisors of 2n-1 and 2n+1
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the smallest prime factor of an odd integer x >= 3.
 * The routine uses trial division up to sqrt(x).  For x prime
 * it returns x itself.  The function works for values that fit
 * into uint64_t, which is sufficient for the intended range of n. */
static uint64_t spf_uint64(uint64_t x)
{
    if ((x & 1ULL) == 0)            /* even case, not expected for our use */
        return 2ULL;

    if (x % 3ULL == 0ULL) return 3ULL;

    /* trial divide by 5,7,9,... skipping even numbers */
    for (uint64_t p = 5ULL; (unsigned __int128)p * p <= x; p += 2ULL) {
        if (x % p == 0ULL)
            return p;
    }
    return x;                       /* x itself is prime */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    /* Compute 2n-1 and 2n+1 safely using 128‑bit arithmetic
       to avoid overflow for very large n (the result is explicitly
       casted back to uint64_t because the numbers we need to factor
       are guaranteed to fit into 64 bits for the usual test ranges). */
    __int128 two_n = ( __int128 )n * 2;
    uint64_t m1 = (uint64_t)(two_n - 1);
    uint64_t m2 = (uint64_t)(two_n + 1);

    uint64_t spf1 = spf_uint64(m1);
    uint64_t spf2 = spf_uint64(m2);

    /* The average fits in seq_t; cast each component to seq_t first. */
    seq_t sum = (seq_t)spf1 + (seq_t)spf2;
    return sum / 2;
}
