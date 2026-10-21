/*
 * A062349  a(n) = n! / (number of prime divisors of n, counted with
 *          multiplicity).
 * https://oeis.org/A062349
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! / bigomega(n) where bigomega is the number of prime
 *            divisors of n counted with multiplicity. The function works for
 *            all n ≥ 2, is memoised across calls and uses only integer
 *            arithmetic
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

/* cached smallest prime factor and bigomega tables */
static idx_t *spf = NULL;
static idx_t *omega = NULL;
static idx_t spf_limit = 0;

/* cached factorial table */
static seq_t *facts = NULL;
static idx_t facts_limit = 0;
static idx_t facts_cap   = 0;

/* ensure spf and omega arrays are computed up to at least n */
static void ensure_spf_omega(idx_t n)
{
    if (n <= spf_limit)
        return;

    /* rebuild spf array up to n */
    if (spf) free(spf);
    build_spf(n, &spf);

    /* recompute bigomega from scratch */
    if (omega) free(omega);
    omega = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    omega[0] = 0;
    omega[1] = 0;
    for (idx_t i = 2; i <= n; ++i)
        omega[i] = 1 + omega[i / spf[i]];

    spf_limit = n;
}

/* ensure factorial table contains values up to at least n */
static void ensure_facts(idx_t n)
{
    if (n <= facts_limit)
        return;

    /* initialise storage if it doesn't exist yet */
    if (facts_cap == 0) {
        facts_cap = 2;
        facts = (seq_t*)malloc(facts_cap * sizeof(seq_t));
        if (!facts) exit(1);
        facts[0] = 1;            /* 0! = 1 */
        facts[1] = 1;            /* 1! = 1 */
        facts_limit = 1;
    }

    /* grow capacity if necessary */
    if (facts_cap <= n) {
        idx_t new_cap = facts_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(facts, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        facts = tmp;
        facts_cap = new_cap;
    }

    /* compute new factorials */
    seq_t val = facts[facts_limit];
    for (idx_t i = facts_limit + 1; i <= n; ++i) {
        val = val * (seq_t)i;
        facts[i] = val;
    }
    facts_limit = n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* behaviour below the offset is undefined */

    ensure_spf_omega(n);
    ensure_facts(n);

    return facts[n] / (seq_t)omega[n];
}
