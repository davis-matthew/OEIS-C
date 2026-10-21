/*
 * A240815  a(n) = index of first nonexisting term of the meta-Fibonacci
 *          sequence {f(1) = ... = f(n) = 1; f(k)=f(k-f(k-3))+f(k-f(k-n))} if
 *          that sequence is only defined for finitely many terms, or 0 if
 *          that sequence is infinite.
 * https://oeis.org/A240815
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = index of first nonexisting term of the meta‑Fibonacci
 *            sequence defined by f(1)=…=f(n)=1; f(k)=f(k‑f(k‑3))+f(k‑f(k‑n))
 *            (a(n) = 0 if the sequence is infinite)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;      /* memoised a(n) values */
static bool   *a_done  = NULL;      /* flags */
static size_t a_cap   = 0;          /* current capacity */

/* resize memoisation tables to hold indices up to at least n */
static void ensure_a_cap(idx_t n)
{
    if ((size_t)n < a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_done  = (bool  *)realloc(a_done,  new_cap * sizeof(bool));
    for (size_t i = a_cap; i < new_cap; ++i) a_done[i] = false;
    a_cap = new_cap;
}

static const size_t MAX_K = 2000000;   /* safety limit for detecting infinity */

/* compute a(n) by direct simulation of f(k) */
static seq_t compute_a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* behaviour below offset irrelevant */

    size_t cap_f = (size_t)n + 5;
    seq_t *F = (seq_t *)malloc(cap_f * sizeof(seq_t));
    if (!F) return (seq_t)0;  /* memory failure; fall back to 0 */

    for (idx_t i = 1; i <= n; ++i) F[i] = (seq_t)1;

    idx_t k = n + 1;  /* next index to compute */

    while (k <= MAX_K) {
        if ((size_t)k >= cap_f) {
            size_t new_cap = cap_f << 1;
            while (new_cap <= (size_t)k) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(F, new_cap * sizeof(seq_t));
            if (!tmp) { free(F); return (seq_t)0; }
            F = tmp;
            cap_f = new_cap;
        }

        /* indices that f(k) refers to */
        idx_t i1 = k - (idx_t)F[k - 3];
        idx_t i2 = k - (idx_t)F[k - n];

        /* failure check: indices must be positive and less than k */
        if (i1 < 1 || i2 < 1 || i1 >= k || i2 >= k)
            return (seq_t)k;  /* first missing term */

        /* compute f(k) */
        F[k] = F[i1] + F[i2];
        ++k;
    }

    /* exceeded safety limit: treat as infinite */
    free(F);
    return (seq_t)0;
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_a_cap(n);
    if (a_done[n]) return a_cache[n];
    seq_t result = compute_a(n);
    a_cache[n] = result;
    a_done[n] = true;
    return result;
}
