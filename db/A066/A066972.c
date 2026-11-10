/*
 * A066972  Duplicate of A067819.
 * https://oeis.org/A066972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = sigma(binomial(2n,n)), the sum of divisors, via
 *            Legendre's formula for prime exponents
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t two_n = 2 * n;
    char *comp = (char *)calloc((size_t)two_n + 1, 1);
    if (!comp) return (seq_t)0;

    seq_t result = (seq_t)1;
    for (idx_t p = 2; p <= two_n; ++p) {
        if (comp[(size_t)p]) continue;
        if (p <= two_n / p) {
            for (idx_t j = p * p; j <= two_n; j += p) comp[(size_t)j] = 1;
        }
        /* exponent of p in binomial(2n, n) */
        idx_t e = 0;
        idx_t pw = p;
        while (true) {
            e += two_n / pw - 2 * (n / pw);
            if (pw > two_n / p) break;
            pw *= p;
        }
        if (e == 0) continue;
        seq_t term = (seq_t)1, cur = (seq_t)1;
        for (idx_t i = 1; i <= e; ++i) {
            cur *= (seq_t)p;
            term += cur;
        }
        result *= term;
    }
    free(comp);
    return result;
}
