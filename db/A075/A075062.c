/*
 * A075062  Row sums of triangle in A075059.
 * https://oeis.org/A075062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (LCM(1..n) + 1) + n * (n - 1) / 2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static idx_t prime_limit = 0;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n = 0 */

    /* ensure we have all primes up to n */
    if (n > prime_limit) {
        free(prime_list);
        build_primes_upto(n, &prime_list, &prime_count);
        prime_limit = n;
    }

    /* compute L = lcm(1..n) */
    seq_t L = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        /* compute exponent e = floor(log_p(n)) */
        idx_t e = 0;
        idx_t x = n;
        while (x >= p) {
            x /= p;
            ++e;
        }
        if (e == 0) continue;                 /* not needed for n < p */
        seq_t factor = (seq_t)1;
        for (idx_t j = 0; j < e; ++j)
            factor *= (seq_t)p;
        L *= factor;
    }

    seq_t seq_n = (seq_t)n;
    seq_t term1 = seq_n * (L + (seq_t)1);
    seq_t term2 = seq_n * (seq_n - (seq_t)1) / (seq_t)2;
    return term1 + term2;
}
