/*
 * A125870  Odd numbers k such that cos(2*Pi/k) is an algebraic number of a
 *          7-smooth degree, but not 5-smooth.
 * https://oeis.org/A125870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth odd integer k such that cos(2*Pi/k)
 * has 7‑smooth algebraic degree but not 5‑smooth.
 * The degree of cos(2π/k) for odd k is φ(k)/2; we test that
 * φ(k)/2 is 7‑smooth (prime factors only 2,3,5,7) and contains 7.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Checks whether the odd integer k satisfies the required conditions. */
static bool is_good_k(uint64_t k)
{
    if ((k & 1ULL) == 0)          /* k must be odd */
        return false;

    /* Factor k: primes in pr[], exponents in ex[] (max 64 factors). */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);

    /* Compute φ(k) = Π p^(e-1)*(p-1). */
    uint64_t phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        phi *= (p - 1);
        for (uint64_t j = 1; j < e; ++j)
            phi *= p;
    }

    /* φ(k) must be divisible by 14 (i.e., include 2 and 7). */
    if (phi % 14ULL != 0ULL)
        return false;

    uint64_t d = phi / 2ULL;   /* degree of cos(2π/k) */

    /* d must contain 7. */
    if (d % 7ULL != 0ULL)
        return false;

    /* Remove all 2,3,5,7 factors from d. */
    while (d % 2ULL == 0ULL) d /= 2ULL;
    while (d % 3ULL == 0ULL) d /= 3ULL;
    while (d % 5ULL == 0ULL) d /= 5ULL;
    while (d % 7ULL == 0ULL) d /= 7ULL;

    /* d should be 1 after stripping 2,3,5,7. */
    return d == 1ULL;
}

/* Returns the nth term (1‑based) of sequence A125870. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, harmless */

    uint64_t count     = 0;
    uint64_t candidate = 3ULL;   /* start from the first odd >1 */

    while (1) {
        if (is_good_k(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2ULL;       /* next odd integer */
    }
}
