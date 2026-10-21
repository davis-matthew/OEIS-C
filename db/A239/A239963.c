/*
 * A239963  Number of triangular numbers below prime(n) which are also
 *          primitive roots modulo prime(n).
 * https://oeis.org/A239963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triangular numbers T_k = k(k+1)/2 < prime(n) *
 *            that are primitive roots modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 7109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Prime for the given index */
    uint32_t p32 = get_prime(n);
    seq_t p      = (seq_t)p32;
    seq_t p_minus_1 = p - 1;

    /* Distinct prime factors of p-1 */
    uint64_t pf[16];
    size_t pf_cnt = 0;
    uint64_t temp = (uint64_t)p_minus_1;
    while (temp > 1) {
        uint64_t spf = _smallest_prime_factor(temp);
        pf[pf_cnt++] = spf;
        while (temp % spf == 0) temp /= spf;
    }

    /* Count triangular numbers that are primitive roots */
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t >= p) break;

        bool is_prim = true;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t exp = (uint64_t)p_minus_1 / pf[i];
            seq_t r = mod_pow(t, exp, p);
            if (r == (seq_t)1) {          /* not a primitive root */
                is_prim = false;
                break;
            }
        }
        if (is_prim) count++;
    }

    return count;
}
