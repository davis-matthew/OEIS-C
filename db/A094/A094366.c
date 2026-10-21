/*
 * A094366  a(n) is the number of two-generated numerical semigroups whose
 *          Frobenius number is 2n-1.
 * https://oeis.org/A094366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # of unordered coprime pairs {a,b} with a<b and
 *            (a‑1)(b‑1)=2n
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here we count factor pairs x*y = 2n with x<y and gcd(x+1,y+1)=1, where
   x = a‑1, y = b‑1.  That exactly equals the required number of
   two‑generated numerical semigroups with Frobenius number 2n‑1. */
#include <stdint.h>

static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification; a(0) is unused */
    if (n == 0) return 0;          /* safety, though never called */
    idx_t twoN = n * 2;            /* product (a‑1)(b‑1) = 2n */
    seq_t count = 0;
    for (idx_t i = 1; i * i < twoN; ++i) {
        if (twoN % i == 0) {
            idx_t j = twoN / i;   /* complementary factor, j > i */
            if (ugcd(i + 1, j + 1) == 1)
                ++count;
        }
    }
    return count;
}
