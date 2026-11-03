/*
 * A118116  a(n) = n concatenated n^2 times.
 * https://oeis.org/A118116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n concatenated n^2 times
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is defined for n ≥ 1 */
        return 0;

    /* compute the number of decimal digits of n */
    idx_t tmp = n;
    idx_t d   = 0;
    while (tmp > 0) {
        tmp /= 10;
        ++d;
    }

    /* 10^d as seq_t (pow10 is supplied by the utils library) */
    seq_t pow10_len = pow10(d);

    /* exponent k = n^2 (fits in idx_t for the tested n ranges) */
    idx_t k = n * n;

    /* fast repeated concatenation by exponentiation by squaring */
    seq_t res        = 0;          /* accumulated result */
    seq_t cur        = (seq_t)n;   /* current block (one copy)     */
    seq_t cur_power  = pow10_len;  /* 10^(len of cur)              */

    while (k > 0) {
        if (k & 1) {
            /* append cur to res: res = res * cur_power + cur */
            res = res * cur_power + cur;
        }
        /* double cur: cur = cur * cur_power + cur */
        cur = cur * cur_power + cur;

        /* cur_power represents 10^(len * current_repeat)
           doubling cur doubles the repeat count, so square cur_power */
        cur_power = cur_power * cur_power;

        k >>= 1;
    }

    return res;
}
