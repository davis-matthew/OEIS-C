/*
 * A078615  a(n) = rad(n)^2, where rad is the squarefree kernel of n
 *          (A007947).
 * https://oeis.org/A078615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p|n} p^2 (radical squared)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t pr[64], exp[64];
    size_t cnt = 0;
    /* factor_u64 fills pr[] with distinct primes of N */
    factor_u64(N, pr, exp, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        result *= p * p;
    }
    return result;
}
