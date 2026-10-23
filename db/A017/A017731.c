/*
 * A017731  Binomial coefficients C(n,67).
 * https://oeis.org/A017731
 *
 * Offset:    67    (a(67) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,67)
 * Range:     u8 n <= 68, u16 n <= 70, u32 n <= 74, u64 n <= 86, u128 n <= 131,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 67; below the offset the result
       is undefined but the function simply returns 0 in that case. */
    const idx_t k = 67;
    if (n < k) return 0;

    /* Cache primes up to the largest n requested so far. */
    static idx_t max_cached = 0;
    static uint64_t *cached_primes = NULL;
    static size_t cached_cnt = 0;

    if (n > max_cached) {
        if (cached_primes) free(cached_primes);
        build_primes_upto(n, &cached_primes, &cached_cnt);
        max_cached = n;
    }

    seq_t result = 1;

    /* For each prime p <= n compute the exponent in C(n,k). */
    for (size_t i = 0; i < cached_cnt; ++i) {
        uint64_t p = cached_primes[i];
        if (p > n) break;

        idx_t exp_n = 0;
        idx_t power = p;
        while (power <= n) {
            exp_n += n / power;
            if (power > n / p) break;   /* avoid overflow */
            power *= p;
        }

        idx_t exp_k = 0;
        power = p;
        while (power <= k) {
            exp_k += k / power;
            if (power > k / p) break;
            power *= p;
        }

        idx_t exp_nk = 0;
        power = p;
        while (power <= n - k) {
            exp_nk += (n - k) / power;
            if (power > (n - k) / p) break;
            power *= p;
        }

        idx_t e = exp_n - exp_k - exp_nk;
        if (e > 0) {
            seq_t p_pow = seq_pow((seq_t)p, e);
            result = seq_mul(result, p_pow);
        }
    }

    return result;
}
