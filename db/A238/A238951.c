/*
 * A238951  Number of arcs from odd to even level vertices in divisor lattice
 *          D(n).
 * https://oeis.org/A238951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, Ω(d) odd} (number of distinct prime factors of
 *            n/d)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Ω(d) is the total number of prime factors of d (with multiplicity). */

/* Required helpers from the utils library */
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include <stdint.h>
#include <stdlib.h>

/* Recursive enumeration of all divisors of n. */
static void
gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t np,
             size_t idx, uint64_t cur, uint64_t *divs, size_t *pos)
{
    if (idx == np) {
        divs[*pos] = cur;
        ++(*pos);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(pr, ex, np, idx + 1, val, divs, pos);
        val *= p;
    }
}

/* Computes the n-th term of OEIS sequence A238951. */
seq_t a(idx_t n)
{
    /* Factor n into prime powers. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t np = 0;
    factor_u64((uint64_t)n, primes, exps, &np);

    /* Count total number of divisors to size array. */
    size_t divcnt = 1;
    for (size_t i = 0; i < np; ++i) {
        divcnt *= (size_t)(exps[i] + 1);
    }

    /* Allocate array for divisors. */
    uint64_t *divs = (uint64_t *)malloc(divcnt * sizeof(uint64_t));
    if (!divs) {
        /* Allocation failed; return 0 as a safe default. */
        return 0;
    }

    /* Generate all divisors. */
    size_t pos = 0;
    gen_divisors(primes, exps, np, 0, 1ULL, divs, &pos);

    /* Sum the desired contributions. */
    seq_t sum = 0;
    for (size_t i = 0; i < pos; ++i) {
        uint64_t d = divs[i];
        if ((bigomega(d) & 1ULL) == 0) {
            continue;            /* Ω(d) even: skip */
        }
        uint64_t co = (uint64_t)n / d;
        seq_t cnt = (seq_t)count_distinct_primes(co);
        sum += cnt;
    }

    free(divs);
    return sum;
}
