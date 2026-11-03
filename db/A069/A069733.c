/*
 * A069733  Number of divisors d of n such that d or n/d is odd. Number of
 *          non-orientable coverings of the Klein bottle with n lists.
 * https://oeis.org/A069733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = number of divisors d of n such that d or n/d is odd.
 *            Equivalent to: if n is odd, a(n) = τ(n); if n is even, let m = n
 *            / 2^e be the odd part of n, then a(n) = 2 * τ(m)
 * Range:     u8 n <= 460, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of divisors of an odd integer m (m <= ULONG_MAX). */
static seq_t odd_divisor_count(seq_t m)
{
    seq_t tau = 1;
    if (m == 0) return 0;          /* defensive: should not occur */
    /* trial division over odd factors, m is odd */
    while (m % 3ULL == 0ULL) {
        int exp = 0;
        do { m /= 3ULL; ++exp; } while (m % 3ULL == 0ULL);
        tau *= (seq_t)(exp + 1);
    }
    seq_t p = 5;
    seq_t step = 2;
    while (p * p <= m) {
        while (m % p == 0ULL) {
            int exp = 0;
            do { m /= p; ++exp; } while (m % p == 0ULL);
            tau *= (seq_t)(exp + 1);
        }
        p += step;
        step = 6 - step;          /* cycle through 5,7,11,13,... */
    }
    if (m > 1ULL) tau *= 2;       /* remaining prime factor */
    return tau;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe */
    /* Count trailing zeros to get exponent of 2 */
    uint64_t e = 0;
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        ++e;
    }
    /* m is now the odd part of n */
    seq_t tau = odd_divisor_count(m);
    if (e == 0) {
        return tau;
    } else {
        return tau * 2;
    }
}
