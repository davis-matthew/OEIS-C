/*
 * A133946  a(n) = Sum phi(k), where the sum is over the integers k which are
 *          the "non-isolated divisors" of 2n and phi(k) is the Euler totient
 *          function (phi(k) = A000010(k)). A positive divisor k of n is
 *          non-isolated if k-1 and/ or k+1 also divides n.
 * https://oeis.org/A133946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum phi(k) over all non‑isolated divisors k of 2n.
 *            Non‑isolated divisor k satisfies k-1 | 2n or k+1 | 2n
 * Range:     u8 n <= 7139, u16 n <= 9937, u32 n <= 9937, u64 n <= 9937,
 *            u128 n <= 9937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper that traverses all divisors of N
   defined by the prime factors in 'pr' (length 'cnt') and
   their maximal exponents in 'exp'.  'sum' accumulates
   phi(k) for the non‑isolated divisors. */
static void dfs_sum(uint8_t idx, uint64_t curr,
                    const uint64_t *pr, const uint8_t *exp,
                    uint8_t cnt, uint64_t N, seq_t *sum)
{
    if (idx == cnt) {
        bool nonisolated = false;
        if (curr > 1 && N % (curr - 1) == 0)
            nonisolated = true;
        else if (curr + 1 <= N && N % (curr + 1) == 0)
            nonisolated = true;

        if (nonisolated) {
            seq_t phi = 1;
            uint64_t d = curr;
            for (uint8_t i = 0; i < cnt; ++i) {
                uint64_t p = pr[i];
                if (d % p == 0) {
                    uint8_t e = 0;
                    while (d % p == 0) {
                        d /= p;
                        ++e;
                    }
                    /* term = p^e - p^(e-1) = p^(e-1) * (p-1) */
                    seq_t p_pow = 1;
                    for (uint8_t k = 0; k < e; ++k)
                        p_pow *= (seq_t)p;
                    seq_t term = p_pow - p_pow / (seq_t)p;
                    phi *= term;
                }
            }
            *sum += phi;
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t cur = curr;
    for (uint8_t e = 0; e <= exp[idx]; ++e) {
        dfs_sum(idx + 1, cur, pr, exp, cnt, N, sum);
        if (e < exp[idx])
            cur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safety */
    uint64_t N = (uint64_t)n * 2ULL;   /* 2n */

    /* Factorize N into primes and exponents */
    uint64_t pr[16];
    uint8_t  exp[16];
    uint8_t  cnt = 0;
    uint64_t tmp = N;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[cnt]  = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        pr[cnt]  = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    seq_t sum = 0;
    dfs_sum(0, 1, pr, exp, cnt, N, &sum);
    return sum;
}
