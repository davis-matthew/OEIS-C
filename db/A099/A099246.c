/*
 * A099246  Denominator of relative frequency of number of ones in the binary
 *          representation of n.
 * https://oeis.org/A099246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, base
 * Formula:   a(n) = A000225(n) / gcd(A000225(n), A000120(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A000225(n) = number of bits in the binary representation of n
    and A000120(n) = popcount(n).  For n = 0 the value is 1. */

#include <stdint.h>

/* popcount for idx_t ----------------------------------------------------- */
static inline idx_t popcnt(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* bit length (A000225) for idx_t ---------------------------------------- */
static inline idx_t bitlen(idx_t x)
{
    /* x > 0 when this function is called */
    return 64 - __builtin_clzll((unsigned long long)x);
}

/* Euclidean gcd for idx_t ------------------------------------------------- */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* sequence function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t pop = popcnt(n);
    idx_t len = bitlen(n);
    idx_t g   = gcd_idx(len, pop);
    return (seq_t)(len / g);
}
