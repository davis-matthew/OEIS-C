/*
 * A158464  Number of distinct squares in row n of Pascal's triangle.
 * https://oeis.org/A158464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct squares among the binomial
 *            coefficients C(n,k) for k = 0..n (Pascal's triangle rows)
 * Range:     u8 n <= 9637, u16 n <= 9637, u32 n <= 9637, u64 n <= 9637,
 *            u128 n <= 9637, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Get the list of primes <= n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Pre‑compute vp(n!) for each prime */
    int *vp_n = (int *)malloc(prime_cnt * sizeof(int));
    if (!vp_n) { free(primes); return (seq_t)0; }           /* fallback on failure */

    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t power = p;
        int sum = 0;
        while (power <= n) {
            sum += n / power;
            power *= p;
        }
        vp_n[i] = sum;
    }

    seq_t count = (seq_t)0;
    idx_t half = n / 2;
    for (idx_t i = 0; i <= half; ++i) {
        bool is_square = true;
        for (idx_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            /* exponent of p in i! */
            uint64_t power = p;
            int exp_i = 0;
            while (power <= i) {
                exp_i += i / power;
                power *= p;
            }
            /* exponent of p in (n-i)! */
            uint64_t power2 = p;
            int exp_j = 0;
            idx_t ni = n - i;
            while (power2 <= ni) {
                exp_j += ni / power2;
                power2 *= p;
            }
            /* exponent in C(n,i) = vp(n!) - vp(i!) - vp((n-i)!) */
            int e = vp_n[j] - exp_i - exp_j;
            if (e & 1) {   /* odd exponent -> not a square */
                is_square = false;
                break;
            }
        }
        if (is_square) {
            /* Count each square only once (k <= n-k guarantees uniqueness) */
            ++count;
        }
    }

    free(vp_n);
    free(primes);
    return count;
}
