/*
 * A013571  Expansion of e.g.f. exp(arctan(x)/exp(x)).
 * https://oeis.org/A013571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = exp(arctan(x)/exp(x)) as an e.g.f.
 * a(n) is the integer coefficient of x^n in the power series of
 * exp(arctan(x)/exp(x)) multiplied by n!.
 * The recurrence is:
 *   a(0) = 1
 *   a(n) = Σ_{k=1..n} C(n‑1, k‑1) * c(k) * a(n‑k)
 * where c(k) = Σ_{i odd ≤ k} C(k, i) * (i‑1)! *
 *           ( (−1)^((i-1)/2 + k‑i) ).
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build system. */
extern seq_t seq_pow(seq_t a, idx_t e); /* not used but keeps file self‑contained */

/* static cache for the sequence values */
static seq_t *A = NULL;      /* a(n) values */
static seq_t *C = NULL;      /* c(k) values (inner series numerators) */
static seq_t *fact = NULL;   /* factorials: fact[i] = i! */
static size_t capacity = 0;  /* allocated size of A, C, fact arrays */
static idx_t max_n = 0;      /* largest n that has been computed */

/* Ensure the allocation can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < capacity)
        return;

    size_t new_cap = (size_t)(n + 1) * 2;
    seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    seq_t *newC = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
    seq_t *newF = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!newA || !newC || !newF) {
        abort();
    }
    A   = newA;
    C   = newC;
    fact = newF;
    capacity = new_cap;
}

/* Ensure factorials up to n-1 are computed (i.e. fact[(n-1)] exists). */
static void ensure_factorials(idx_t n)
{
    static idx_t fact_cap = 0;       /* number of stored factorials */
    if (fact_cap == 0) {
        fact[0] = 1;
        fact_cap = 1;
    }
    while (fact_cap <= n) {
        seq_t val = fact[fact_cap - 1] * (seq_t)fact_cap;
        fact[fact_cap] = val;
        ++fact_cap;
    }
}

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    if (max_n == 0) {            /* first call – initialize */
        ensure_capacity(0);
        A[0] = 1;
        C[0] = 0;                /* not used but harmless */
        fact[0] = 1;
        capacity = 1;
        max_n = 0;
    }

    if (n <= max_n) {
        return A[n];
    }

    ensure_capacity(n);
    ensure_factorials(n);

    for (idx_t idx = max_n + 1; idx <= n; ++idx) {
        /* ----- compute c(idx) ----- */
        seq_t sum_c = 0;
        seq_t binom = 1;                 /* binom(idx,0) */
        for (idx_t i = 1; i <= idx; ++i) {
            /* update binom(idx,i) from binom(idx,i-1) */
            seq_t num = (seq_t)(idx - i + 1);
            binom = binom * num / (seq_t)i;
            if ((i & 1) != 0) {          /* odd i */
                int parity = ((i - 1) / 2 + (idx - i)) & 1;
                seq_t term = binom * fact[i - 1];
                if (parity) term = -term;
                sum_c += term;
            }
        }
        C[idx] = sum_c;

        /* ----- compute a(idx) ----- */
        seq_t sum_a = 0;
        seq_t binom_nm1 = 1;            /* binom(idx-1,0) for k=1 */
        for (idx_t k = 1; k <= idx; ++k) {
            seq_t term = binom_nm1 * C[k];
            term = term * A[idx - k];
            sum_a += term;
            if (k < idx) {
                seq_t num = (seq_t)(idx - k);
                seq_t den = (seq_t)k;
                binom_nm1 = binom_nm1 * num / den;
            }
        }
        A[idx] = sum_a;
    }
    max_n = n;
    return A[n];
}
