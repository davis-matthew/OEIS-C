/*
 * A211507  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2=x*y-n.
 * https://oeis.org/A211507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y) | 1 ≤ w,x,y ≤ n, w^2 = x*y - n}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper recursive function that enumerates all divisors of P
   given its prime factorisation.  For each divisor d we check
   whether d ≤ N and P/d ≤ N; if so the corresponding ordered
   pair (d, P/d) contributes one to the count. */
static void count_rec(const uint64_t *pr, const uint8_t *ex,
                      size_t k, size_t idx,
                      uint64_t cur, uint64_t P, uint64_t N,
                      uint64_t *cnt)
{
    if (idx == k) {
        if (cur <= N && P / cur <= N)
            ++(*cnt);
        return;
    }
    uint64_t val = 1;
    for (uint8_t e = 0; e <= ex[idx]; ++e) {
        count_rec(pr, ex, k, idx + 1, cur * val,
                  P, N, cnt);
        val *= pr[idx];
    }
}

/* Counts the number of ordered pairs (x,y) in [1,N]^2
   satisfying x*y = P.  It uses trial division to
   obtain the prime factorisation of P and then
   enumerates all divisors.  The result is returned
   through the pointer *cnt. */
static void count_divisors(uint64_t P, uint64_t N, uint64_t *cnt)
{
    /* 32 is far more than enough for n ≤ 10⁴ */
    static uint64_t primes[32];
    static uint8_t exps[32];
    size_t k = 0;
    uint64_t m = P;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint8_t e = 0;
            do {
                m /= p;
                ++e;
            } while (m % p == 0);
            primes[k] = p;
            exps[k] = e;
            ++k;
        }
    }
    if (m > 1) {
        primes[k] = m;
        exps[k] = 1;
        ++k;
    }

    *cnt = 0;
    count_rec(primes, exps, k, 0, 1, P, N, cnt);
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    /* The index may be 0; for the offset the sequence starts at n = 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t total = 0;
    uint64_t N = (uint64_t)n;          /* integer arithmetic in 64‑bit */

    /* We only need to examine w in 1 … n */
    for (idx_t wi = 1; wi <= n; ++wi) {
        uint64_t w = (uint64_t)wi;
        uint64_t P = w * w + N;        /* w² + n = x·y */

        /* If P > N² then any y = P/x with x ≤ N exceeds N, so skip */
        if (P > N * N)
            continue;

        uint64_t cnt = 0;
        count_divisors(P, N, &cnt);
        total += (seq_t)cnt;
    }

    return total;
}
