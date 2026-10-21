/*
 * A073180  Number of divisors of n which are not greater than the squarefree
 *          kernel of n.
 * https://oeis.org/A073180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n with d ≤ squarefree kernel of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Static globals used by the divisor‑generation recursion. */
static uint64_t rad_global;
static uint64_t count_global;

/* Recursive helper: generate all divisors of n from its prime factorization.
   pr   – array of distinct primes,
   exp  – array of their exponents,
   idx  – current position in arrays,
   cnt  – total number of distinct primes,
   cur  – current divisor being built.
   Divisors ≤ rad_global are counted in count_global. */
static void dfs_div(uint64_t cur, size_t idx, size_t cnt, uint64_t *pr, int *exp)
{
    if (idx == cnt) {
        if (cur <= rad_global)
            ++count_global;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs_div(val, idx + 1, cnt, pr, exp);
        val *= pr[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t tmp = (uint64_t)n;
    uint64_t pr[64];
    int exp[64];
    size_t cnt = 0;

    /* Factorization by trial division. */
    if (tmp % 2 == 0) {
        pr[cnt] = 2;
        exp[cnt] = 0;
        while (tmp % 2 == 0) {
            exp[cnt]++;
            tmp /= 2;
        }
        ++cnt;
    }
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            pr[cnt] = d;
            exp[cnt] = 0;
            while (tmp % d == 0) {
                exp[cnt]++;
                tmp /= d;
            }
            ++cnt;
        }
    }
    if (tmp > 1) {
        pr[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    /* Squarefree kernel (product of distinct primes). */
    rad_global = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad_global *= pr[i];

    /* Count divisors ≤ rad_global. */
    count_global = 0;
    dfs_div(1, 0, cnt, pr, exp);

    return (seq_t)count_global;
}
