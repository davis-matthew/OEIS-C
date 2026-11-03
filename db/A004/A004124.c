/*
 * A004124  Discriminant of n-th cyclotomic polynomial.
 * https://oeis.org/A004124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = Sign(a(n))*(n^phi(n))/(Product_{p|n, p prime}
 *            p^(phi(n)/(p-1))), Sign(a(n)) = (-1)^(phi(n)*(phi(n)-1)/2);
 *            computed as Product_{p^e||n} p^(e*phi(n) - phi(n)/(p-1))
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 10, i64 n <= 18, i128 n <= 28,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 387 (2026-11-01)
 * Helpers:   ipow_idx, totient_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient_idx.h"

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    idx_t ph = totient_idx(n);
    seq_t result = 1;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; e++; }
            idx_t ex = e * ph - ph / (p - 1);
            result *= ipow_idx((seq_t)p, ex);
        }
    }
    if (t > 1) {
        idx_t ex = ph - ph / (t - 1);
        result *= ipow_idx((seq_t)t, ex);
    }
    /* phi*(phi-1)/2 is odd iff phi mod 4 is 2 or 3 */
    if ((ph & 3) >= 2) result = -result;
    return result;
}
