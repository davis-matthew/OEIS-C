/*
 * A136166  a(n) = the number of divisors of n that are each not coprime to
 *          the previous or the following divisor of n.
 * https://oeis.org/A136166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n that are not coprime to at
 *            least one of its neighbours in the sorted list of divisors
 * Range:     u8 n <= 277, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic system headers. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* GCD helper from the utils library. */
#include "../utils/gcd.h"

/* Helper to compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) - (x > y);
}

/* Recursive generation of all divisors. */
static void gen_divs_rec(uint64_t cur, size_t idx,
                         uint64_t *pr, uint64_t *ex, size_t pcnt,
                         uint64_t *divs, size_t *pos)
{
    if (idx == pcnt) {
        divs[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t exp = ex[idx];
    uint64_t val = cur;
    for (uint64_t k = 0; k <= exp; ++k) {
        gen_divs_rec(val, idx + 1, pr, ex, pcnt, divs, pos);
        if (k < exp)
            val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition. */
    if (n <= 1ULL)
        return (seq_t)0ULL;

    /* ----------------- Factor n ------------------------------------ */
    /* Allocate temporary arrays for prime factors and exponents. */
    /* 64 distinct primes are more than enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    /* Simple trial division factorisation (sufficient for n ≤ 1e12). */
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* ---------------- Divisor count --------------------------------- */
    size_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        divcnt *= (size_t)(exps[i] + 1);

    /* Allocate array of divisors. */
    uint64_t *divs = (uint64_t *)malloc(divcnt * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0ULL;          /* allocation failed: return 0 */

    /* Generate all divisors. */
    size_t pos = 0;
    gen_divs_rec(1ULL, 0, primes, exps, pcnt, divs, &pos);

    /* ----------------- Sort the divisors --------------------------- */
    qsort(divs, pos, sizeof(uint64_t), cmp_uint64);

    /* ---------------- Count qualifying divisors -------------------- */
    seq_t result = 0;
    for (size_t i = 0; i < pos; ++i) {
        bool good = false;
        if (i > 0 && gcd((seq_t)divs[i], (seq_t)divs[i - 1]) > (seq_t)1)
            good = true;
        if (i + 1 < pos && gcd((seq_t)divs[i], (seq_t)divs[i + 1]) > (seq_t)1)
            good = true;
        if (good)
            result++;
    }

    /* Clean up. */
    free(divs);

    return result;
}
