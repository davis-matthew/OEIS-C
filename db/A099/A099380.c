/*
 * A099380  The imaginary part of -n', the arithmetic derivative for Gaussian
 *          integers.
 * https://oeis.org/A099380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n * Σ_{p^e || n} e * a(p)/p where a(p) = 0 if p ≡ 3 (mod
 *            4), a(2) = 2, for primes p ≡ 1 (mod 4) a(p) = a + b, with p = a²
 *            + b² (a ≥ b > 0)
 * Range:     u8 n <= 63, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    /* Search over the range [0, 2³²] because (2³²)² < 2⁶⁴. */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;
    if (high > n) high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* Mid satisfies mid*mid > n  if  mid > n/mid. */
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* a(p) for a prime number p.  Finds the unique representation
   p = a² + b² for primes p ≡ 1 (mod 4). */
static inline seq_t calc_a_prime(idx_t p)
{
    if (p == 2) return (seq_t)2;
    if (p % 4 == 3) return 0;

    /* p ≡ 1 (mod 4) */
    idx_t a = isqrt_u64(p);
    while (a >= 1) {
        idx_t rem = p - a * a;
        idx_t b = isqrt_u64(rem);
        if (b * b == rem && b > 0)
            return (seq_t)(a + b);
        --a;
    }
    return 0; /* should never happen for p ≡ 1 (mod 4) primes */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t result = 0;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t ap = calc_a_prime(p);
            seq_t term = (seq_t)exp * ap * (seq_t)(n / p);
            result += term;
        }
    }

    if (temp > 1) {                 /* remaining prime factor */
        seq_t ap = calc_a_prime(temp);
        seq_t term = (seq_t)ap * (seq_t)(n / temp);
        result += term;
    }

    return result;
}
