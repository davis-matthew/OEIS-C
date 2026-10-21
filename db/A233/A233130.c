/*
 * A233130  Number of negative formulas with two connectives (-> and *) and no
 *          variables.
 * https://oeis.org/A233130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 a(n)=∑_{k=0}^{n-1} (A151374(k)-a(k))·a(n-k-1)
 *            A151374(k)=Catalan(k)·2^k (Catalan numbers are the usual Catalan
 *            sequence)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Lazy memoisation of the sequence. The static variables hold the
   values for all n that have been requested so far. The sequence is
   built incrementally, so calls may be in any order. */
static seq_t *a_seq   = NULL;   /* a(n) values                */
static seq_t *tot_seq = NULL;   /* A151374(n) = Catalan(n)*2^n */
static size_t a_cap   = 0;      /* allocated size for a_seq    */
static size_t t_cap   = 0;      /* allocated size for tot_seq  */
static idx_t  max_computed = (idx_t)(-1); /* last n that has been computed */
static seq_t catalan_cur;       /* Catalan(n) for current n    */
static seq_t pow2_cur;          /* 2^n for current n          */

/* Helper to grow a dynamic array; 'needed' is the smallest required
   size (non‑zero). */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap) * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (max_computed == (idx_t)(-1)) {
        /* a(0) */
        ensure_capacity(&a_seq, &a_cap, 1);
        a_seq[0] = 1;

        /* Catalan(0)=1, 2^0=1 */
        catalan_cur = (seq_t)1;
        pow2_cur   = (seq_t)1;

        /* A151374(0)=1 */
        ensure_capacity(&tot_seq, &t_cap, 1);
        tot_seq[0] = catalan_cur * pow2_cur;

        max_computed = 0;
    }

    /* if nothing further to compute, return the already known value */
    if (n <= max_computed)
        return a_seq[n];

    /* enlarge the arrays if necessary */
    ensure_capacity(&a_seq, &a_cap, (size_t)n + 1);
    if (n > 0)
        ensure_capacity(&tot_seq, &t_cap, (size_t)n);

    /* --------------------------------------------------------------------
       Build values up to n.  We first compute the intermediate indices
       1..n‑1 because their Catalans, pow‑2 factors and A151374 values
       are needed by the recurrence for a(n).  After the loop we
       compute a(n) itself and then update the auxiliary
       Catalan/pow‑2 state for future extensions. southern
     -------------------------------------------------------------------- */
    for (idx_t m = max_computed + 1; m < n; ++m) {
        /* Catalan(m) from Catalan(m-1) */
        catalan_cur = catalan_cur * (seq_t)(4 * m - 2) / (seq_t)(m + 1);

        /* compute a(m) */
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t diff = tot_seq[k] - a_seq[k];
            seq_t prod = diff * a_seq[m - 1 - k];
            sum += prod;
        }
        a_seq[m] = sum;

        /* pow2 and A151374 for index m */
        pow2_cur <<= 1;                    /* 2^m   */
        ensure_capacity(&tot_seq, &t_cap, (size_t)m + 1);
        tot_seq[m] = catalan_cur * pow2_cur;
    }

    /* -------- Compute a(n) ------------------------------------------- */
    seq_t sum_n = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t diff = tot_seq[k] - a_seq[k];
        seq_t prod = diff * a_seq[n - 1 - k];
        sum_n += prod;
    }
    a_seq[n] = sum_n;

    /* -------- Update state for possible larger n ---------------------- */
    /* Catalan(n) from Catalan(n-1) */
    catalan_cur = catalan_cur * (seq_t)(4 * n - 2) / (seq_t)(n + 1);
    /* 2^n  */
    pow2_cur <<= 1;
    if (n >= t_cap) {
        size_t new_cap = (t_cap > 0) ? t_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        tot_seq = (seq_t*)realloc(tot_seq, new_cap * sizeof(seq_t));
        t_cap   = new_cap;
    }
    tot_seq[n] = catalan_cur * pow2_cur;

    max_computed = n;
    return a_seq[n];
}
