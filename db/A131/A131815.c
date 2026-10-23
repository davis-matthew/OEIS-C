/*
 * A131815  The 2n-th derivative of exp(1 - 1/(1 - x^2)) evaluated at 0.
 * https://oeis.org/A131815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131815
   a(n) = the 2n-th derivative of exp(1-1/(1-x²)) evaluated at 0.
   Recurrence:
     a(0) = 1
     a(n) = -(1/n) * Σ_{k=1}^n k*a(n-k)*(2n)!/(2n-2k)!
   The coefficients a(n) are the factorial times the coefficient of
   y^n in exp(-y/(1-y)) with y = x².
   The implementation builds factorials and a(n) values on demand,
   with memoisation suitable for arbitrary call order. */

#include <stdlib.h>

/* factorial cache: fact[i] = i!  (i ≤ fact_len)              */
static seq_t *fact = NULL;
static size_t fact_cap = 0;
static idx_t fact_len = 0;  /* highest index currently computed   */

/* sequence cache: a_seq[n] contains a(n)   */
static seq_t *a_seq = NULL;
static size_t a_cap = 0;
static idx_t a_len = 0;      /* highest index currently computed   */

/* Ensure factorials up to `upto` (inclusive) are computed.        */
static void ensure_factorials(idx_t upto)
{
    if (!fact) {
        fact_cap = 1;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact) exit(EXIT_FAILURE);
        fact[0] = (seq_t)1;
        fact_len = 0;
    }
    if (upto <= fact_len) return;

    size_t new_cap = fact_cap;
    while (new_cap <= upto) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    fact = tmp;
    fact_cap = new_cap;

    for (idx_t i = fact_len + 1; i <= upto; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_len = upto;
}

/* Ensure a(n) values up to `upto` (inclusive) are computed.      */
static void ensure_a(idx_t upto)
{
    if (!a_seq) {
        a_cap = 8;
        if (a_cap < upto + 1) a_cap = upto + 1;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_seq) exit(EXIT_FAILURE);
    }
    if (upto <= a_len) return;

    if (upto + 1 > a_cap) {
        size_t new_cap = a_cap;
        while (new_cap <= upto) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_seq = tmp;
        a_cap = new_cap;
    }

    /* initialise first term if not yet set */
    if (a_len == 0 && a_seq[0] == 0) a_seq[0] = (seq_t)1;

    for (idx_t n = a_len + 1; n <= upto; ++n) {
        ensure_factorials(n * 2);          /* need (2n)! and (2n-2k)!   */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = fact[2 * n];
            term /= fact[2 * n - 2 * k];   /* exact division            */
            term *= a_seq[n - k];
            term *= (seq_t)k;
            sum += term;
        }
        a_seq[n] = -(sum / (seq_t)n);
    }
    a_len = upto;
}

/* External linkage function as required.                        */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_a(n);
    return a_seq[n];
}
