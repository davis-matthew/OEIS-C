/*
 * A064406  Partial sums of A064405.
 * https://oeis.org/A064406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n+1)(n+2)/2 – 2 * Σ_{k=0..n} 2^{popcount(k)}
 * Range:     i8 n <= 34, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Thread‑local cache of prefix sums Σ_{k=0..i} 2^{popcount(k)} */
static seq_t *pref   = NULL;
static size_t pref_len = 0;   /* number of already filled entries */
static size_t pref_cap = 0;   /* allocated capacity */

/* Ensure that pref[0..n] exist (pref_len >= n+1) */
static void ensure_pref(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (pref_len >= need) return;            /* already available */

    /* Grow allocation if necessary */
    if (pref == NULL) {
        pref_cap = need > 1 ? need : 1;
        pref     = (seq_t *)malloc(pref_cap * sizeof(seq_t));
        if (!pref) abort();
        pref[0]    = (seq_t)1;        /* 2^{popcount(0)} = 1 */
        pref_len   = 1;
    } else if (pref_cap < need) {
        pref_cap = need > pref_cap * 2 ? need : pref_cap * 2;
        pref = (seq_t *)realloc(pref, pref_cap * sizeof(seq_t));
        if (!pref) abort();
    }

    /* Fill missing entries */
    for (size_t k = pref_len; k < need; ++k) {
        unsigned pop = __builtin_popcountll((unsigned long long)k);
        seq_t term   = ((seq_t)1 << pop);
        pref[k] = pref[k-1] + term;
    }
    pref_len = need;
}

seq_t a(idx_t n)
{
    ensure_pref(n);

    /* T(n) = (n+1)(n+2)/2 */
    seq_t nn   = (seq_t)n;
    seq_t T    = (nn + 1) * (nn + 2) / 2;

    /* result = T - 2 * pref[n] */
    seq_t result = T - ((seq_t)2 * pref[(size_t)n]);

    return result;
}
