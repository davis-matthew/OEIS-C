/*
 * A126836  Ramanujan numbers (A000594) read mod 7.
 * https://oeis.org/A126836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*sigma_3(n) mod 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* below offset, unused */

    /* n mod 7 */
    seq_t n_mod = (seq_t)(n % 7);

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute sigma_3(n) mod 7 */
    seq_t sigma_mod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t p_mod7 = (seq_t)(primes[i] % 7);

        /* p^3 mod 7 */
        seq_t p3_mod7 = (p_mod7 * p_mod7) % 7;
        p3_mod7 = (p3_mod7 * p_mod7) % 7;

        /* sum_{k=0..e} (p^3)^k mod 7 */
        seq_t term = (seq_t)1;
        seq_t sum  = (seq_t)1;            /* k = 0 term */
        for (uint64_t k = 1; k <= e; ++k) {
            term = (seq_t)((term * p3_mod7) % 7);
            sum  = (seq_t)((sum + term) % 7);
        }

        sigma_mod = (seq_t)((sigma_mod * sum) % 7);
    }

    /* final result: n*sigma_3(n) mod 7 */
    seq_t result = (seq_t)((n_mod * sigma_mod) % 7);
    return result;
}
