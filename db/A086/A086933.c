/*
 * A086933  Number of solutions to x^2 + y^2 = 0 mod n.
 * https://oeis.org/A086933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) is multiplicative with a(2^e) = 2^e, a(p^e) = p^(e-e%2) if
 *            p ≡ 3 (mod 4), a(p^e) = ((p-1)*e + p)·p^(e-1) if p ≡ 1 (mod 4)
 *            and p ≠ 2. The function factors n and multiplies the
 *            contributions
 * Range:     u8 n <= 84, u16 n <= 8124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation for seq_t (works for unsigned 8/16/32/64 or arbitrary-precision) */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;      /* n fits into 64 bits */
    uint64_t e;

    /* Factor 2 */
    if (m & 1ULL) {               /* m is odd, skip */
    } else {
        e = 0;
        while ((m & 1ULL) == 0) {
            e++;
            m >>= 1;
        }
        if (e > 0)
            result *= pow_seq_t((seq_t)2, e);
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        e = 0;
        while (m % p == 0) {
            e++;
            m /= p;
        }
        if (p % 4 == 3) {   /* p ≡ 3 (mod 4) */
            uint64_t exp = e - (e & 1ULL);   /* e - (e % 2) */
            result *= pow_seq_t((seq_t)p, exp);
        } else {           /* p ≡ 1 (mod 4) */
            seq_t part1 = (seq_t)((p - 1) * e + p);
            seq_t part2 = pow_seq_t((seq_t)p, e > 0 ? e - 1 : 0);
            result *= part1 * part2;
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        uint64_t p = m;            /* exponent = 1 */
        if (p % 4 == 3) {
            /* e = 1, p^(1-1) = 1 → no contribution */
        } else {                   /* p ≡ 1 (mod 4) */
            seq_t part1 = (seq_t)((p - 1) * 1 + p);   /* 2p - 1 */
            result *= part1;      /* part2 = p^0 = 1 */
        }
    }

    return result;
}
