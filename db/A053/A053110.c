/*
 * A053110  Expansion of (-1 + 1/(1-7*x)^7)/(49*x); related to A036226.
 * https://oeis.org/A053110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^(n-1) * binomial(n+7, 6), with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of OEIS A053110 */
seq_t a(idx_t n) {
    /* a(0) is defined as 1 by the OEIS entry */
    if (n == 0) return 1;

    /* Compute binomial(n+7, 6) = (n+7)(n+6)(n+5)(n+4)(n+3)(n+2) / 720 */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        /* Numerator term: n + 8 - i */
        binom = (binom * (seq_t)(n + 8 - i)) / (seq_t)i;
    }

    /* Compute 7^(n-1) using binary exponentiation */
    seq_t power = 1;
    seq_t base  = 7;
    idx_t exp   = n - 1;          /* n is at least 1 here */
    while (exp) {
        if (exp & 1) power = power * base;
        base = base * base;
        exp >>= 1;
    }

    /* Final result */
    return binom * power;
}
