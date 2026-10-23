/*
 * A160499  Number of quartic primitive Dirichlet characters modulo n.
 * https://oeis.org/A160499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) is multiplicative with a(2)=0, a(4)=1, a(8)=2, a(16)=4;
 *            a(2^e)=0 for e=1 or e>=5; for odd primes p, a(p)=3 if p≡1
 *            (mod4), a(p)=1 if p≡3 (mod4), and a(p^e)=0 for e≥2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    uint64_t val = (uint64_t)n;
    seq_t result = 1;

    /* factor out powers of 2 */
    int e2 = 0;
    while ((val & 1ULL) == 0) {
        ++e2;
        val >>= 1;
    }
    if (e2) {
        if (e2 == 1 || e2 >= 5) return 0;
        if (e2 == 2) result *= 1;      // a(4)
        else if (e2 == 3) result *= 2; // a(8)
        else if (e2 == 4) result *= 4; // a(16)
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= val / p; p += 2) {
        if (val % p == 0) {
            int e = 0;
            while (val % p == 0) {
                ++e;
                val /= p;
                if (e > 1) return 0;   // odd prime power >1 gives zero
            }
            if (p % 4 == 1) {
                result *= 3;           // a(p) for p≡1 mod4
            }                         // p≡3 mod4 contributes factor 1
        }
    }

    /* remaining factor is a prime > sqrt(original n) */
    if (val > 1) {
        if (val % 4 == 1) result *= 3;
    }

    return result;
}
