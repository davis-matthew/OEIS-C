/*
 * A048447  Take the first n numbers written in base 16, concatenate them,
 *          then convert from base 16 to base 10.
 * https://oeis.org/A048447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the base‑16 representations of * 1,2,…,n
 *            interpreted as a single base‑16 number and converted to *
 *            base‑10. This is equivalent to * a(n) = (((((0 * 16^d1 + 1) *
 *            16^d2 + 2) * 16^d3 + 3) … * 16^dn + n), * where di is the number
 *            of hexadecimal digits of i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t    res = 0;            /* current value */
    seq_t    pow_digit = (seq_t)16;   /* 16^digits, updated at thresholds */
    idx_t    next_threshold = 16;     /* first value that changes digits */

    for (idx_t k = 1; k <= n; ++k) {
        /* update the power when we reach a new digit length */
        if (k == next_threshold) {
            next_threshold *= (idx_t)16;
            pow_digit *= (seq_t)16;
        }
        res = res * pow_digit + (seq_t)k;
    }
    return res;
}
