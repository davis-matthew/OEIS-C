/*
 * A084859  Binomial transform of Cullen numbers A002064.
 * https://oeis.org/A084859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 2*n*3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for seq_t base raised to a non‑negative integer exp. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp) {            /* avoid a useless final multiplication when exponent becomes zero */
            base = base * base;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* 2^0 + 0 = 1 */

    /* 2^n */
    seq_t p2 = pow_seq((seq_t)2, n);

    /* 3^(n-1) */
    seq_t p3 = pow_seq((seq_t)3, n - 1);

    /* 2 * n * 3^(n-1) */
    seq_t term = (seq_t)n * p3;
    term = term * (seq_t)2;       /* multiply by 2 */

    return p2 + term;
}
