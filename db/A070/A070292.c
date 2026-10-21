/*
 * A070292  a(n) = lcm(12,n)/gcd(12,n).
 * https://oeis.org/A070292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(12,n)/gcd(12,n) = 12*n/gcd(12,n)^2 Equivalent integer
 *            form: a(n) = (12/gcd) * (n/gcd)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For safety – the sequence is defined for n >= 1 */
    if (n == 0) return 0;

    uint64_t g = gcd64(12ULL, (uint64_t)n);          /* g = gcd(12,n)   */
    uint64_t n_div = (uint64_t)n / g;                /* n/g              */
    uint64_t mult  = 12ULL / g;                     /* 12/g in {12,6,4,3,2,1} */
    uint64_t res64 = mult * n_div;                  /* product fits in 64‑bit for tested n */

    return (seq_t)res64;
}
