/*
 * A082344  Denominator of sopfr(n)/n, where sopfr=A001414 is the sum of prime
 *          factors (with repetition).
 * https://oeis.org/A082344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, sopfr(n))
 * Range:     u8 n <= 257, u16 n <= 9795, u32 n <= 9795, u64 n <= 9795,
 *            u128 n <= 9795, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sopfr(n) = sum of prime factors of n (with multiplicity) */

#include <stdint.h>

/* Compute gcd of two seq_t values */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Sum of prime factors of n (with multiplicity) */
static seq_t sopfr_uint64(uint64_t n)
{
    seq_t sum = 0;
    while (n % 2ULL == 0ULL) {
        sum += 2;
        n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0ULL) {
            sum += p;
            n /= p;
        }
    }
    if (n > 1) {
        sum += n;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1, behavior for n<1 is irrelevant */
    seq_t seq_n = (seq_t)n;
    seq_t sp = sopfr_uint64((uint64_t)n);
    seq_t g   = seq_gcd(seq_n, sp);
    return seq_n / g;
}
