/*
 * A175323  a(n) = the largest divisor of n that has only distinct run-lengths
 *          (among all runs both of 0's and of 1's) when written in binary.
 * https://oeis.org/A175323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest divisor d of n such that d's binary
 *            representation has all run lengths of consecutive 0's and 1's
 *            distinct
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   good_run_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_run_div.h"

/* Factor n into primes and exponents. */
static void factorize(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int c = 0;
    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }
        primes[c] = 2; exps[c] = e; ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[c] = p; exps[c] = e; ++c;
        }
    }
    if (n > 1) { primes[c] = n; exps[c] = 1; ++c; }
    *cnt = c;
}

/* Recursively generate all divisors from prime powers. */
static void gen_divs(int idx, int total, uint64_t curr,
                     const uint64_t *primes, const int *exps,
                     uint64_t *arr, int *ptr)
{
    if (idx == total) {
        arr[*ptr] = curr;
        (*ptr)++;
        return;
    }
    uint64_t mult = 1ULL;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, total, curr * mult, primes, exps, arr, ptr);
        mult *= primes[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t N = n;
    if (N == 0) return (seq_t)0;  /* not used, safe guard */

    /* Factor N */
    uint64_t primes[64];
    int exps[64];
    int pc;
    factorize(N, primes, exps, &pc);

    /* Generate all divisors */
    uint64_t divisors[4096];
    int dc = 0;
    gen_divs(0, pc, 1ULL, primes, exps, divisors, &dc);

    /* Find the largest divisor satisfying the run–length property */
    uint64_t best = 1;
    for (int i = 0; i < dc; ++i) {
        uint64_t d = divisors[i];
        if (good_run_div(d) && d > best) best = d;
    }

    return (seq_t)best;
}
