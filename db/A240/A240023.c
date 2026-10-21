/*
 * A240023  Product of factorials of prime divisors of n (with multiplicity).
 * https://oeis.org/A240023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   build_spf, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240023: Product of factorials of prime divisors of n (multiplicity)
   a(n) = ∏_{p|n} (p!)^{v_p(n)}   for n ≥ 1
   a(1) = 1                            (empty product) */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* build_spf */
#include "../utils/seq_mul.h"     /* seq_mul */
#include "../utils/seq_pow.h"     /* seq_pow */

/* Cached smallest‑prime‑factor array and its limit. */
static idx_t *g_spf = NULL;
static idx_t   g_spf_limit = 0;

/* Ensure that the spf array covers all integers up to `limit`. */
static void ensure_spf(idx_t limit)
{
    if (limit <= g_spf_limit)
        return;
    idx_t *new_spf = NULL;
    build_spf(limit, &new_spf);
    if (g_spf) free(g_spf);
    g_spf = new_spf;
    g_spf_limit = limit;
}

/* Compute p! for a prime p, using the seq_* helpers. */
static seq_t factorial_of_prime(idx_t p)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= p; ++i)
        res = seq_mul(res, (seq_t)i);
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, but no harm */
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    /* Prepare spf up to n. */
    ensure_spf(n);

    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m > 1) {
        idx_t p = g_spf[m];
        idx_t cnt = 0;
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }

        seq_t fact_p = factorial_of_prime(p);
        seq_t power = seq_pow(fact_p, cnt);
        result = seq_mul(result, power);
    }

    return result;
}
