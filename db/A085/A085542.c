/*
 * A085542  Determinant of the n X n matrix M_(i,j)=i/gcd(i,j)=lcm(i,j)/j.
 * https://oeis.org/A085542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{p ≤ n, p prime} (1 - p)^{⌊n/p⌋}
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* undefined for n=0; return 1 to avoid UB */

    uint64_t *primes = NULL;
    size_t primes_cnt;
    build_primes_upto(n, &primes, &primes_cnt);

    seq_t result = 1;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p64 = primes[idx];
        seq_t p     = (seq_t)p64;            /* convert prime to seq_t */
        seq_t exp   = (seq_t)(n / p64);      /* exponent floor(n/p) */

        if (exp == 0) continue;

        seq_t base = (seq_t)1 - p;          /* base = 1 - p  (negative) */
        seq_t pow  = big_pow(base, exp);
        result = result * pow;              /* multiply into running product */
    }

    free(primes);
    return result;
}
