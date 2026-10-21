/*
 * A227782  Stufe of Q(sqrt(-n)): least number of squares which add to -1 in
 *          the field Q(sqrt(-n)).
 * https://oeis.org/A227782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227782
   Formula: a(n) = 1 if n is a perfect square;
            a(n) = 4 if n = 4^k · (8m + 7) for some k ≥ 0, m ≥ 0;
            a(n) = 2 otherwise. */

#include <stdint.h>

/* Integer floor square root for uint64_t using bitwise algorithm.
   This avoids any floating‑point arithmetic and never overflows. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t bit = 1ULL << 62;         /* the largest power of four ≤ 2^64-1 */
    while (bit > n)
        bit >>= 2;

    uint64_t result = 0;
    while (bit != 0) {
        if (n >= result + bit) {
            n -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for the offset, safe default */

    /* 1. check for perfect square */
    uint64_t s = isqrt_u64(n);
    if (s * s == n)
        return (seq_t)1;

    /* 2. strip factors of 4: n = 4^k · m, m odd */
    uint64_t m = n;
    while ((m & 3ULL) == 0)   /* while m divisible by 4 */
        m >>= 2;

    /* 3. check congruence modulo 8 */
    if ((m & 7ULL) == 7)
        return (seq_t)4;

    /* 4. remaining case */
    return (seq_t)2;
}
