/*
 * A202606  a(n) = ceiling(((10^n - 1)^2/9 + 10^n)/18).
 * https://oeis.org/A202606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((10^n – 1)/9) * ((10^n – 1)/9 + 1)/2 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A202606.
 * Uses the provided pow10() utility to obtain 10^n as a seq_t value.
 * Then evaluates
 *   k = (10^n – 1)/9
 *   a = k*(k+1)/2 + 1
 * which is mathematically equivalent to the original definition but
 * uses only integer arithmetic on seq_t. */
 
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline seq_t tri_half_carry(seq_t x, seq_t y) {
    /* compute x*y/2 without overflow when possible by dividing first */
    if ((x & ((seq_t)1)) == 0) {
        x >>= 1;
        return x * y;
    } else {
        y >>= 1;
        return x * y;
    }
}

seq_t a(idx_t n)
{
    /* 10^n as seq_t */
    seq_t p = pow10(n);

    /* k = (10^n – 1)/9 */
    seq_t one = (seq_t)1;
    seq_t k   = (p - one) / (seq_t)9;

    /* a = k*(k+1)/2 + 1 */
    seq_t kp1  = k + one;
    seq_t prod = tri_half_carry(k, kp1);
    seq_t res  = prod + one;

    return res;
}
