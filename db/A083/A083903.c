/*
 * A083903  Number of divisors of n with largest digit <= 8 (base 10).
 * https://oeis.org/A083903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n with largest digit ≤ 8 (base 10)
 * Range:     u8 n <= 328, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_nine, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/contains_nine.h"

static void count_no9(uint64_t *pr, uint64_t *ex, size_t cnt, size_t idx, uint64_t cur, int *res) {
    if (idx == cnt) {
        if (!contains_nine((seq_t)cur))
            (*res)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (int e = 0; e <= (int)ex[idx]; ++e) {
        count_no9(pr, ex, cnt, idx + 1, cur * val, res);
        val *= p;
    }
}

seq_t a(idx_t n) {
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    int res = 0;
    count_no9(primes, exps, cnt, 0, 1ULL, &res);
    return (seq_t)res;
}
