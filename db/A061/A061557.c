/*
 * A061557  a(n) = (7*n+2)*C(n)/(n+2), where C(n) is the n-th Catalan number.
 * https://oeis.org/A061557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*C(n+1) - C(n)
 * where C(k) is the k-th Catalan number.
 * Computation uses a multiplicative algorithm that never overflows
 * when C(k) fits into seq_t.
 */

#include <stdint.h>

/* Compute greatest common divisor for seq_t values. */
static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Return the k-th Catalan number C(k). */
static seq_t catalan(idx_t k)
{
    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t a = (seq_t)k + i;   /* numerator factor */
        seq_t b = i;              /* denominator factor */

        /* cancel common factors between res and b */
        seq_t g1 = _gcd_seq_t(res, b);
        if (g1 != (seq_t)1) {
            res /= g1;
            b /= g1;
        }

        /* cancel remaining common factors between a and b */
        seq_t g2 = _gcd_seq_t(a, b);
        if (g2 != (seq_t)1) {
            a /= g2;
            b /= g2;
        }

        res *= a;                 /* multiply by reduced numerator */

        if (b != (seq_t)1)        /* divide by any remaining factor */
            res /= b;
    }

    /* divide by (k+1) to get C(k) = binom(2k,k)/(k+1) */
    seq_t div = (seq_t)k + (seq_t)1;
    seq_t g3 = _gcd_seq_t(res, div);
    if (g3 != (seq_t)1) {
        res /= g3;
        div /= g3;
    }
    if (div != (seq_t)1)
        res /= div;

    return res;
}

/* Main sequence function called from the framework. */
seq_t a(idx_t n)
{
    /* n >= 1 by the problem statement */
    seq_t c_n   = catalan(n);
    seq_t c_np1 = catalan(n + 1);

    seq_t result = c_np1;
    result += c_np1;
    result -= c_n;

    return result;
}
