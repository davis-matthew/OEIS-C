/*
 * A038388  Let f(n) be the smallest number such that the arithmetic mean (A)
 *          and geometric mean (G) of n and f(n) are both integers; sequence
 *          gives G values.
 * https://oeis.org/A038388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e: * if p == 2: *
 *            a(2^e) = 2^{(e+1)/2} if e is odd, * a(2^e) = 2^{e/2 + 1} if e is
 *            even; * otherwise (p > 2): * a(p^e) = p^{(e+1)/2} if e is odd, *
 *            a(p^e) = p^{e/2} if e is even. * The function returns the value
 *            of a(n) for n >= 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small exponentiation for seq_t values. */
static inline seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;

    seq_t res = 1;
    idx_t m = n;

    for (idx_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }

            seq_t term;
            if (i == 2) {
                if (e & 1)           /* e odd */
                    term = seq_pow((seq_t)2, (e + 1) / 2);
                else                  /* e even */
                    term = seq_pow((seq_t)2, e / 2 + 1);
            } else {
                if (e & 1)           /* e odd */
                    term = seq_pow((seq_t)i, (e + 1) / 2);
                else                  /* e even */
                    term = seq_pow((seq_t)i, e / 2);
            }
            res *= term;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        seq_t term = seq_pow((seq_t)m, 1);  /* exponent is 1 (odd) */
        res *= term;
    }

    return res;
}
