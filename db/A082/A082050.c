/*
 * A082050  Sum of divisors of n that are not of the form 3k+1.
 * https://oeis.org/A082050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d%3!=1} d (sum of divisors of n not ≡1 mod 3)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9007 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper to enumerate all divisors using the factorization.
   pr[]   : array of prime factors (uint64_t)
   exps[] : corresponding exponents
   count  : number of distinct prime factors
   idx    : current recursion depth
   current: current divisor value built so far
   sum    : pointer to running sum of qualifying divisors */
static void dfs_rec(const uint64_t *pr, const int *exps, int count,
                    int idx, seq_t current, seq_t *sum)
{
    if (idx == count) {
        if ((current % (seq_t)3) != (seq_t)1)
            *sum += current;
        return;
    }
    seq_t val = current;
    for (int k = 0; k <= exps[idx]; ++k) {
        dfs_rec(pr, exps, count, idx + 1, val, sum);
        val *= (seq_t)pr[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n<1, return 0 */

    /* Factorization: simple trial division */
    uint64_t temp = n;
    uint64_t pr[64];
    int exps[64];
    int pc = 0;

    int e = 0;
    while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
    if (e > 0) {
        pr[pc] = 2;
        exps[pc] = e;
        ++pc;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        if (e > 0) {
            pr[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (temp > 1) {
        pr[pc] = temp;
        exps[pc] = 1;
        ++pc;
    }

    seq_t sum = 0;
    dfs_rec(pr, exps, pc, 0, (seq_t)1, &sum);
    return sum;
}
