/*
 * A220026  The period with which the powers of n repeat mod 1000000.
 * https://oeis.org/A220026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the minimal positive t such that n^i ≡ n^{i+t} (mod
 *            1000000) for all i ≥ 0. For n coprime to 10 the exponent equals
 *            the multiplicative order of n modulo 1000000. Otherwise the
 *            period is the lcm of the orders modulo 2^6 and 5^6, after the
 *            powers of 2 or 5 have become constant
 * Range:     u8 n <= 0, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast modular exponentiation for small moduli */
static seq_t pow_mod(seq_t base, seq_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* gcd for seq_t values */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* lcm for seq_t values (both operands non‑zero) */
static seq_t lcm_seq(seq_t a, seq_t b) {
    return a / gcd_seq(a, b) * b;
}

seq_t a(idx_t n) {
    /* a(0) is defined to be 1 in the OEIS entry */
    if (n == 0)
        return 1;

    /* count powers of 2 in n */
    idx_t tmp = n;
    int e2 = 0;
    while ((tmp & 1ULL) == 0ULL && tmp)
        tmp >>= 1, ++e2;

    /* count powers of 5 in n */
    tmp = n;
    int e5 = 0;
    while ((tmp % 5ULL) == 0ULL && tmp)
        tmp /= 5ULL, ++e5;

    seq_t period2, period5;

    /* period modulo 2^6 = 64 */
    if (e2 == 0) {               /* n coprime to 2 */
        seq_t base = (seq_t)(n % 64ULL);
        seq_t t = 32;            /* φ(64) = 32 */
        while (t % 2 == 0 && pow_mod(base, t / 2, 64) == 1)
            t /= 2;
        period2 = t;
    } else {
        period2 = 1;             /* becomes 0 after enough powers */
    }

    /* period modulo 5^6 = 15625 */
    if (e5 == 0) {               /* n coprime to 5 */
        seq_t base = (seq_t)(n % 15625ULL);
        seq_t t = 12500;         /* φ(15625) = 12500 = 2^2*5^5 */
        while (t % 2 == 0 && pow_mod(base, t / 2, 15625) == 1)
            t /= 2;
        while (t % 5 == 0 && pow_mod(base, t / 5, 15625) == 1)
            t /= 5;
        period5 = t;
    } else {
        period5 = 1;             /* becomes 0 after enough powers */
    }

    return lcm_seq(period2, period5);
}
