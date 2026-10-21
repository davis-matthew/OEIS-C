/*
 * A074935  Denominator of a(n), where for n > 2, a(n)=-1/a(n-1)+1/a(n-2),
 *          a(1)=1, a(2)=2.
 * https://oeis.org/A074935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = -1/a(n-1) + 1/a(n-2), a(1)=1, a(2)=2 This function
 *            returns the denominator of the reduced fraction a(n)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   add_signed, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/add_signed.h"

static seq_t *numerator_mag = NULL;   /* magnitudes of numerators */
static bool   *numerator_neg = NULL;   /* true if numerator is negative */
static seq_t *denominator    = NULL;   /* denominators, always positive */
static size_t cache_capacity = 0;
static idx_t  last_computed  = 0;

/* Ensure the cache can hold indices 1..n */
static void ensure_cache(idx_t n)
{
    if (n < cache_capacity)
        return;
    size_t new_cap = (size_t)n + 1;
    numerator_mag = (seq_t *)realloc(numerator_mag, new_cap * sizeof(seq_t));
    numerator_neg = (bool   *)realloc(numerator_neg, new_cap * sizeof(bool));
    denominator    = (seq_t *)realloc(denominator   , new_cap * sizeof(seq_t));
    for (size_t i = cache_capacity; i < new_cap; ++i) {
        numerator_mag[i] = 0;
        numerator_neg[i] = false;
        denominator[i]    = 0;
    }
    cache_capacity = new_cap;
}

/* Main function: returns the denominator of a(n) */
seq_t a(idx_t n)
{
    if (n == 1 || n == 2)                 /* first two terms are integers */
        return 1;

    ensure_cache(n);

    /* initialise first two terms when the cache is empty */
    if (last_computed < 2) {
        numerator_mag[1] = (seq_t)1;  numerator_neg[1] = false;  denominator[1] = (seq_t)1;
        numerator_mag[2] = (seq_t)2;  numerator_neg[2] = false;  denominator[2] = (seq_t)1;
        last_computed = 2;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* previous two terms: (p_prev1/q_prev1) = a(i-1), (p_prev2/q_prev2) = a(i-2) */
        seq_t p_prev1 = numerator_mag[i-1];
        bool  neg_p_prev1 = numerator_neg[i-1];
        seq_t p_prev2 = numerator_mag[i-2];
        bool  neg_p_prev2 = numerator_neg[i-2];
        seq_t q_prev1 = denominator[i-1];
        seq_t q_prev2 = denominator[i-2];

        /* Compute the two terms of the recurrence with correct signs */
        seq_t t1mag = q_prev1 * p_prev2;           /* magnitude of -q_prev1/p_prev1 */
        bool  t1neg = !neg_p_prev1;               /* sign of -1/a(i-1) */

        seq_t t2mag = q_prev2 * p_prev1;           /* magnitude of  q_prev2/p_prev2 */
        bool  t2neg = neg_p_prev2;                 /* sign of +1/a(i-2) */

        /* Sum the two signed terms */
        seq_t nummag;  bool numneg;
        add_signed(t1mag, t1neg, t2mag, t2neg, &nummag, &numneg);

        /* Reduce the fraction: denominator_raw = p_prev1 * p_prev2 */
        seq_t g1 = gcd(nummag, p_prev1);
        seq_t m1 = nummag / g1;
        seq_t d1 = p_prev1 / g1;

        seq_t g2 = gcd(m1, p_prev2);
        seq_t d2 = p_prev2 / g2;

        seq_t new_pmag = m1 / g2;      /* reduced numerator magnitude */
        seq_t new_denom = d1 * d2;      /* reduced denominator */

        numerator_mag[i] = new_pmag;
        numerator_neg[i] = numneg;
        denominator[i]   = new_denom;
    }

    last_computed = n;
    return denominator[n];
}
