/*
 * A098228  a(n) = floor(n/(n-phi(n))) = floor(n/cototient(n)).
 * https://oeis.org/A098228
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / (n - phi(n))) = floor(n / cototient(n))
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes for n >= 2. Works for any seq_t and idx_t definitions. */
#pragma once

/* Calculate Euler's totient function phi(n) for n up to 64-bit values. */
static seq_t calc_phi(idx_t n) {
    idx_t result = n;
    idx_t tmp = n;

    if ((tmp & 1) == 0) {                /* handle factor 2 */
        result -= result >> 1;           /* result / 2 */
        while ((tmp & 1) == 0) tmp >>= 1;
    }

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            result -= result / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        result -= result / tmp;
    }
    return (seq_t)result;
}

/* Main sequence function as required. */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* out of defined range */
    seq_t phi = calc_phi(n);
    idx_t denom = n - (idx_t)phi;        /* n - φ(n) > 0 for n >= 2 */
    return (seq_t)(n / denom);           /* floor division */
}
