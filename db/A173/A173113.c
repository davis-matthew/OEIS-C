/*
 * A173113  a(n) = binomial(n + 10, 10) * 5^n.
 * https://oeis.org/A173113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10) * 5^n
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply by 5^exp using exponentiation by squaring */
static seq_t pow5(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 5;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base   = base * base;
        exp  >>= 1;
    }
    return result;
}

/* compute binomial(n+10,10) multiplicatively */
static seq_t binom10(idx_t n) {
    seq_t res = 1;
    /* C(n+10,10) = product_{k=1}^{10} (n+10-k+1)/k */
    for (idx_t k = 1; k <= 10; ++k) {
        seq_t numer = (seq_t)(n + 10 - k + 1); /* (n + 11 - k) */
        res = res * numer / (seq_t)k;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t c = binom10(n);          /* C(n+10,10) */
    seq_t p = pow5(n);             /* 5^n */
    return c * p;
}
