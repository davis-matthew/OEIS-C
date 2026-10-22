/*
 * A002408  Expansion of 8-dimensional cusp form.
 * https://oeis.org/A002408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy, mult
 * Formula:   a(n) is multiplicative with a(2^e) = -8^e if e>0, a(p^e) =
 *            ((p^3)^(e+1) - 1) / (p^3 - 1) = Sum_{k=0..e} p^(3k) for odd p;
 *            a(0) = 0
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t temp = n;

    idx_t e2 = 0;
    while (temp % 2 == 0) {
        e2++;
        temp /= 2;
    }
    if (e2 > 0) {
        seq_t pw = 1;
        for (idx_t k = 0; k < e2; k++) pw = pw * 8;
        result = -pw;
    }

    for (idx_t p = 3; p * p <= temp; p += 2) {
        idx_t e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            seq_t ps = (seq_t)p;
            seq_t p3 = ps * ps * ps;
            seq_t term = 1;
            seq_t p3k = 1;
            for (idx_t k = 1; k <= e; k++) {
                p3k = p3k * p3;
                term = term + p3k;
            }
            result = result * term;
        }
    }

    if (temp > 1) {
        seq_t ps = (seq_t)temp;
        seq_t term = ps * ps * ps + 1;
        result = result * term;
    }

    return result;
}
