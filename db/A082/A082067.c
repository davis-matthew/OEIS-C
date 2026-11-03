/*
 * A082067  Smallest prime that divides n and phi(n)=A000010(n), or 1 if n and
 *          phi(n) are relatively prime.
 * https://oeis.org/A082067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(n, φ(n)) = 1; otherwise the smallest prime
 *            factor of gcd(n, φ(n)). φ is Euler’s totient function
 * Range:     u8 n <= 465, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9346 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

/* Return the smallest prime divisor of a positive integer x.  For x<2
   return 0 (not used).  The routine uses simple trial division. */
static uint64_t spf_uint64(uint64_t x)
{
    if (x < 2) return 0;
    if ((x & 1) == 0) return 2;
    uint64_t i = 3;
    for (; i * i <= x; i += 2) {
        if (x % i == 0) return i;
    }
    return x;   /* x is prime */
}

seq_t a(idx_t n)
{
    seq_t phi_n = totient(n);
    seq_t g = gcd((seq_t)n, phi_n);

    if (g == (seq_t)1) return (seq_t)1;   /* coprime case */

    /* g ≤ n, so it fits in uint64_t */
    uint64_t g_ul = (uint64_t)g;
    uint64_t spf = spf_uint64(g_ul);
    return (seq_t)spf;
}
