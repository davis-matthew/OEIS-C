/*
 * A075453  Numerator of 1/pp(n) + 1/pp(n+1), where pp(n) = A001597(n-1) is
 *          the n-th perfect power.
 * https://oeis.org/A075453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator of (1/pp(n) + 1/pp(n+1)) where pp(n) =
 *            A001597(n-1) is the n‑th perfect power (with pp(1) = 1). The
 *            function returns the reduced numerator of the sum of
 *            reciprocals. It memoises the list of perfect powers up to the
 *            largest n requested
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   cmp_seq_t, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>        /* malloc, free, qsort */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t, seq_t) */
#include "../utils/cmp_seq_t.h" /* int cmp_seq_t(const void*, const void*) */
#include "../utils/floor_sqrt.h"/* seq_t floor_sqrt(seq_t) */

static seq_t *pp = NULL;          /* memoised list of perfect powers (0‑based) */
static size_t pp_len = 0;         /* number of entries stored */

/* rebuild the perfect‑power table so that pp has at least need entries */
static void ensure_pp(idx_t need)
{
    if (pp_len >= need)
        return;   /* already have enough */

    /* limit = (need)^2  (need = n+1) ensures pp[need-1] exists */
    seq_t limit = ((seq_t)need) * ((seq_t)need);

    /* estimate a reasonable capacity: twice the number of squares */
    size_t est_cap = need * 2 + 42;
    seq_t *tmp = (seq_t *)malloc(est_cap * sizeof(seq_t));
    if (!tmp) return;  /* fallback: may crash; problem not expected */

    size_t len = 0;
    tmp[len++] = (seq_t)1;   /* pp(1) = 1 */

    /* maximum base such that base^2 <= limit */
    idx_t max_base = (idx_t)floor_sqrt(limit);

    for (idx_t a = 2; a <= max_base; ++a) {
        seq_t mult = (seq_t)a;
        seq_t val  = mult * mult;        /* a^2 */
        while (val <= limit) {
            if (len == est_cap) {        /* grow if needed */
                est_cap <<= 1;
                seq_t *r = (seq_t *)realloc(tmp, est_cap * sizeof(seq_t));
                if (!r) { free(tmp); return; }
                tmp = r;
            }
            tmp[len++] = val;
            /* compute next exponent; guard against overflow within seq_t */
            if (mult != 0 && val > limit / mult) break;
            val *= mult;
        }
    }

    /* sort and unique */
    qsort(tmp, len, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq-1]) {
            tmp[uniq++] = tmp[i];
        }
    }

    /* replace old table */
    if (pp) free(pp);
    pp     = tmp;
    pp_len = uniq;
}

/* Main entry--exports the required function name. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but guard */
    /* Need pp(n) and pp(n+1): indices n-1 and n in pp array */
    ensure_pp(n + 1);                  /* ensures pp_len >= n+1 */

    seq_t p = pp[n-1];   /* pp(n)   */
    seq_t q = pp[n];     /* pp(n+1) */

    seq_t num = p + q;   /* numerator before reduction      */
    seq_t den = p * q;   /* denominator                      */
    seq_t g   = gcd(num, den);  /* common divisor                   */
    return num / g;                     /* reduced numerator                */
}
