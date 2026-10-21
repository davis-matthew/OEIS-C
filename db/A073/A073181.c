/*
 * A073181  Sum of divisors of n which are not greater than the squarefree
 *          kernel of n.
 * https://oeis.org/A073181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073181: sum of divisors of n that are ≤ the squarefree kernel (rad) of n */
#include <stdint.h>
#include <stddef.h>

/* Simple trial‑division factorisation (sufficient for n up to ~10^12, which covers the
   test ranges). It writes the distinct prime factors into pr[] and their exponents
   into exp[], setting *cnt to the number of factors. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    if (n == 0) {
        *cnt = 0;
        return;
    }
    while ((n & 1ULL) == 0) {
        if (c == 0 || pr[c-1] != 2ULL) {
            pr[c] = 2ULL;
            exp[c] = 0;
            ++c;
        }
        ++exp[c-1];
        n >>= 1;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            pr[c] = p;
            exp[c] = 0;
            ++c;
            while (n % p == 0) {
                ++exp[c-1];
                n /= p;
            }
        }
    }
    if (n > 1) {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of all divisors of n from its prime factorisation.
   For each divisor d, if d ≤ rad (passed as rad_limit), add it to sum. */
static void gen_divs(size_t idx, uint64_t cur,
                     const uint64_t *pr, const uint64_t *exp,
                     size_t cnt, uint64_t rad_limit, uint64_t *sum)
{
    if (idx == cnt) {
        if (cur <= rad_limit)
            *sum += cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t emax = exp[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= emax; ++e) {
        gen_divs(idx + 1, cur * val, pr, exp, cnt, rad_limit, sum);
        if (e < emax)
            val *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t pr[64], exp[64];
    size_t cnt = 0;

    factor_uint64(N, pr, exp, &cnt);

    /* squarefree kernel = product of distinct primes */
    uint64_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= pr[i];

    uint64_t sum = 0;
    gen_divs(0, 1, pr, exp, cnt, rad, &sum);

    return (seq_t)sum;
}
