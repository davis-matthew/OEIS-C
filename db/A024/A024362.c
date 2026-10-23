/*
 * A024362  Number of primitive Pythagorean triangles with hypotenuse n.
 * https://oeis.org/A024362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for odd n, a(n) = 2^(k-1) if every prime factor of n is ≡1
 *            (mod4) where k is the number of distinct prime factors;
 *            otherwise a(n)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* idx_t (index type) and seq_t (value type) are supplied by the build. */
seq_t a(idx_t n)
{
    /* Handle the sequence offset: a(1) = 0, etc. */
    if (n < 1) return (seq_t)0;
    /* Even numbers cannot be hypotenuses of primitive triples. */
    if ((n & 1ULL) == 0) return (seq_t)0;

    uint64_t m = (uint64_t)n;
    int k = 0;          /* number of distinct prime factors ≡1 (mod4) */
    bool bad = false;   /* true if a prime factor ≡3 (mod4) is found */

    /* Trial division over odd candidates up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            /* Prime factor found.  If it is not ≡1 (mod4), the term is zero. */
            if ((p & 3ULL) != 1ULL) {
                bad = true;
                break;
            }
            k++;                     /* count this distinct prime once */
            /* Remove all powers of p */
            while (m % p == 0) m /= p;
        }
    }

    if (bad) return (seq_t)0;

    /* Any remaining prime factor m > 1 (odd, because n is odd) */
    if (m > 1) {
        if ((m & 3ULL) != 1ULL) return (seq_t)0;
        k++;  /* distinct prime factor = m */
    }

    if (k == 0) return (seq_t)0;  /* no qualifying primes, result is 0 */

    /* Compute 2^(k-1) safely in seq_t */
    seq_t val = 1;
    for (int i = 0; i < k - 1; i++) val *= 2;

    return val;
}
