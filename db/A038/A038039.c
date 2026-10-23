/*
 * A038039  a(n) = Sum_{d|n} (2^d*3^(n/d)).
 * https://oeis.org/A038039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (2^d*3^(n/d))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

static void rec(const uint64_t primes[], const uint64_t exps[],
                size_t idx, size_t total, uint64_t curr,
                seq_t *sum, idx_t n) {
    if (idx == total) {
        seq_t t2 = pow2((idx_t)curr);
        seq_t t3 = pow3((idx_t)(n / curr));
        seq_t term = seq_mul(t2, t3);
        *sum += term;
        return;
    }
    uint64_t p  = primes[idx];
    uint64_t e  = exps[idx];
    uint64_t v  = 1;
    for (size_t i = 0; i <= e; ++i) {
        rec(primes, exps, idx + 1, total, curr * v, sum, n);
        v *= p;
    }
}

static size_t factor(uint64_t n, uint64_t primes[], uint64_t exps[]) {
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    static seq_t *mem = NULL;
    static size_t mem_cap = 0;
    if ((idx_t)mem_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        mem = seq_realloc(mem, mem_cap, new_cap);
        for (size_t i = mem_cap; i < new_cap; ++i) mem[i] = (seq_t)0;
        mem_cap = new_cap;
    }
    if (mem[n] != (seq_t)0) return mem[n];

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = factor((uint64_t)n, primes, exps);

    seq_t sum = (seq_t)0;
    rec(primes, exps, 0, pcnt, 1, &sum, n);

    mem[n] = sum;
    return sum;
}
