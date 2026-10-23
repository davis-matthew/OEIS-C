/*
 * A103769  Trinomial transform of central binomial coefficients A001405.
 * https://oeis.org/A103769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..2n} T(n,k)*C(k, floor(k/2)) where T(n,k) is the
 *            trinomial coefficient from (1+x+x²)^n. The implementation builds
 *            the trinomial rows iteratively, keeps a table of central
 *            binomial coefficients, and memoises all sequence values. All
 *            arithmetic is performed with the type seq_t (unsi...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mul128, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul128.h"

/* Memoised sequence values */
static seq_t *a_vals = NULL;    /* array of a(n) */
static idx_t  a_cap  = 0;       /* a_vals allocated capacity */

/* Central binomial coefficients C(m, floor(m/2)) for 0 <= m <= 2*max_n */
static seq_t *central    = NULL;
static idx_t  central_cap = 0;  /* allocated capacity */

/* Trinomial coefficients for the previous row */
static seq_t *T_prev   = NULL;
static idx_t  T_len    = 0;     /* length of T_prev (2*n+1) */
static idx_t  cur_n    = 0;     /* highest n with a_vals[0..cur_n] computed */

/* Ensure central[] has values up to m_max (inclusive) */
static void ensure_central(idx_t m_max)
{
    if (central_cap > m_max) return;

    /* Expand array, keep old values */
    size_t new_cap = (size_t)m_max + 1;
    seq_t *tmp = seq_realloc(central, (idx_t)new_cap);
    if (!tmp) return;          /* allocation failed – silently abort */
    central = tmp;
    /* Compute missing values */
    for (idx_t m = (central_cap > 0 ? central_cap : 0); m <= m_max; ++m) {
        if (m == 0) {
            central[0] = 1;
            continue;
        }
        /* recurrence: C(m,⌊m/2⌋) = C(m‑1,⌊m/2⌋) * m / ceil(m/2) */
        seq_t num = mul128(central[m - 1], (seq_t)m);
        idx_t den = (m + 1) / 2;          /* ceil(m/2) */
        central[m] = num / (seq_t)den;
    }
    central_cap = new_cap;
}

/* Ensure a_vals[0..goal] are computed and memoised */
static void ensure_a(idx_t goal)
{
    /* initialise storage if not yet done */
    if (a_vals == NULL) {
        a_vals = seq_malloc(1);
        if (!a_vals) return;
        a_vals[0] = 1;          /* a(0) = 1 */
        a_cap = 1;

        T_prev = seq_malloc(1);
        if (!T_prev) return;
        T_prev[0] = 1;           /* T(0,0) = 1 */
        T_len = 1;
        cur_n = 0;
    }

    if (goal <= cur_n) return;

    /* central coefficients up to 2*goal are needed */
    ensure_central(2 * goal);

    /* grow a_vals array if necessary */
    if (a_cap <= goal) {
        seq_t *tmp = seq_realloc(a_vals, goal + 1);
        if (!tmp) return;
        a_vals = tmp;
        a_cap = goal + 1;
    }

    /* build successive trinomial rows and a(n) */
    for (idx_t n = cur_n + 1; n <= goal; ++n) {
        idx_t new_len = 2 * n + 1;
        seq_t *T_new = seq_malloc(new_len);
        if (!T_new) return;
        /* compute row n from row n‑1 */
        for (idx_t k = 0; k < new_len; ++k) {
            seq_t val = 0;
            if (k < T_len)             val += T_prev[k];
            if (k >= 1 && k - 1 < T_len) val += T_prev[k - 1];
            if (k >= 2 && k - 2 < T_len) val += T_prev[k - 2];
            T_new[k] = val;
        }
        /* compute a(n) = Σ_k T_new[k] * C(k,⌊k/2⌋) */
        seq_t sum = 0;
        for (idx_t k = 0; k < new_len; ++k) {
            seq_t prod = mul128(T_new[k], central[k]);
            sum += prod;
        }
        a_vals[n] = sum;
        /* next iteration */
        free(T_prev);
        T_prev = T_new;
        T_len  = new_len;
    }
    cur_n = goal;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_vals[n];
}
