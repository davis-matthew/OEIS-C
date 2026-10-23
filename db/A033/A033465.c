/*
 * A033465  Numerators of the first differences of 1/(n^2 + 1).
 * https://oeis.org/A033465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (2*n+1)/((n^2+1)*((n+1)^2+1))
 * Range:     u8 n <= 127, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Euclidean gcd for unsigned 64-bit integers.  The sequence terms
 * fit into seq_t for all n used in the tests, and the helper works
 * with values that may be larger than seq_t. */
static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b)
{
    while (b) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* numerator before reduction */
    unsigned long long anum = 2ULL * n + 1ULL;

    /* compute (n^2 + 1) * ((n+1)^2 + 1) modulo anum */
    unsigned long long n_mod   = n % anum;
    unsigned long long part1   = ((n_mod * n_mod) % anum + 1ULL) % anum;

    unsigned long long nm1     = n + 1ULL;
    unsigned long long nm1_mod = nm1 % anum;
    unsigned long long part2   = ((nm1_mod * nm1_mod) % anum + 1ULL) % anum;

    unsigned long long denom_mod = (part1 * part2) % anum;

    /* reduce the fraction: a(n) = anum / gcd(anum, denom_mod) */
    unsigned long long g      = gcd_ull(anum, denom_mod);
    unsigned long long result = anum / g;

    return (seq_t)result;
}
