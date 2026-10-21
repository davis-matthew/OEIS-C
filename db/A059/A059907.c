/*
 * A059907  a(n) = |{m : multiplicative order of n mod m = 2}|.
 * https://oeis.org/A059907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n^2-1) - tau(n-1)
 * Range:     u8 n <= 1870, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where tau(x) = number of divisors of x */
#include <stdint.h>
#include "../utils/factor_uint64.h"

/* count divisors of a 64‑bit integer */
static seq_t tau_uint64(uint64_t x) {
    uint64_t fac[20];
    int exp[20];
    int cnt;
    /* factor_uint64 fills fac[], exp[] and cnt */
    factor_uint64(x, fac, exp, &cnt);
    seq_t res = 1;
    for (int i = 0; i < cnt; ++i) {
        res *= (seq_t)(exp[i] + 1);
    }
    return res;
}

seq_t a(idx_t n) {
    /* the sequence starts at n = 1 */
    if (n <= 1) return (seq_t)0;

    uint64_t nm1 = (uint64_t)n - 1;
    uint64_t nm1sq = (uint64_t)n * (uint64_t)n - 1;   /* n^2 - 1 */

    seq_t tau1 = tau_uint64(nm1sq);
    seq_t tau2 = tau_uint64(nm1);

    return tau1 - tau2;
}
