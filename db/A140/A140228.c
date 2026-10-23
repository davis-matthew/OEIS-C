/*
 * A140228  Binomial transform of [1, 5, 10, 10, 5, 1, 1, -1, 1, -1, 1, ...].
 * https://oeis.org/A140228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(274 + 85*n^2 + n^4)/60 for n >= 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 191, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b)
    {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Work with 64‑bit integers for the intermediate calculation */
    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;          /* n^2  */
    uint64_t n4 = n2 * n2;          /* n^4  */
    uint64_t t1 = 274ULL + 85ULL * n2 + n4; /* 274 + 85*n^2 + n^4 */

    uint64_t g = gcd64(nn, 60ULL);      /* gcd(n,60) */
    uint64_t d1 = 60ULL / g;            /* divisor after canceling gcd */
    uint64_t t1div = t1 / d1;           /* t1 is guaranteed divisible by d1   */
    uint64_t n1 = nn / g;               /* remaining factor of n             */

    uint64_t result = n1 * t1div;       /* final integer result within 64‑bit */
    return (seq_t)result;
}
