/*
 * A031358  Number of coincidence site lattices of index 4n+1 in lattice Z^2.
 * https://oeis.org/A031358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^k if every prime divisor of 4n+1 is 1 (mod 4), where k
 *            is the number of distinct such primes; otherwise a(n) = 0. a(0)
 *            = 1
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper factor_u64 from ../utils/factor_u64.h to factor 4n+1. */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* 4n+1 fits in uint64_t for the expected n range */
    uint64_t m = 4*n + 1;

    /* Factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* If any prime ≡ 3 (mod 4) appears, the result is 0 */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 4 == 3) {
            return (seq_t)0;
        }
    }

    /* Count distinct primes ≡ 1 (mod 4) */
    int k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 4 == 1) {
            ++k;
        }
    }

    /* Compute 2^k */
    seq_t res = (seq_t)1;
    for (int i = 0; i < k; ++i) {
        res *= (seq_t)2;
    }
    return res;
}
