/*
 * A082408  Least k such that the continued fraction for n/k contains no
 *          element > 2.
 * https://oeis.org/A082408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that the simple
 *            continued fraction expansion of n/k contains only terms ≤ 2
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t start = (n + 2) / 3;          /* ceil(n/3) – first quotient will be ≤ 2 for k ≥ start */
    for (idx_t k = start; k <= n; ++k) {
        seq_t num = (seq_t)n;
        seq_t den = (seq_t)k;
        bool ok = true;
        while (den != 0) {
            seq_t q = num / den;
            if (q > 2) {      /* a term exceeds 2 – not acceptable */
                ok = false;
                break;
            }
            seq_t r = num - q * den;
            num = den;
            den = r;
        }
        if (ok) return (seq_t)k;
    }
    return (seq_t)n;   /* fallback – k = n always works (n/n = 1) */
}
