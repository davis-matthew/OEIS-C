/*
 * A108781  Expansion of sqrt((1-x+8*x^2)/(1-x)^3).
 * https://oeis.org/A108781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 22, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108781: Recurrence
 * n*a(n) + (-2*n+1)*a(n-1) + (9*n-25)*a(n-2) + 4*(-2*n+5)*a(n-3) = 0
 * with a(0)=1, a(1)=1, a(2)=5.  The function memoizes computed terms. */
#include <stdlib.h>

/* cache of computed terms */
static seq_t *seq = NULL;
static idx_t  seq_cap = 0;   /* allocated capacity (in elements) */
static idx_t  seq_len = 0;   /* number of initialized entries */

/* ensure capacity for indices up to n (inclusive) */
static void ensure_cap(idx_t n)
{
    if (seq_cap > n) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(*seq));
    if (!tmp) exit(1);   /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* already computed? */
    if (n < seq_len) return seq[n];

    /* allocate and compute missing terms */
    ensure_cap(n);
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0) {
            seq[i] = (seq_t)1;
        } else if (i == 1) {
            seq[i] = (seq_t)1;
        } else if (i == 2) {
            seq[i] = (seq_t)5;
        } else {
            seq_t n_seq = (seq_t)i;
            /* coefficients from the recurrence */
            seq_t coef1 = ((seq_t)(-2) * n_seq) + (seq_t)1;     /* -2*n+1    */
            seq_t coef2 = ((seq_t)9 * n_seq) - (seq_t)25;       /* 9*n-25    */
            seq_t coef3 = (seq_t)4 * (((seq_t)(-2) * n_seq) + (seq_t)5); /* 4*(-2*n+5) */
            seq_t numerator = -(coef1 * seq[i-1] + coef2 * seq[i-2] + coef3 * seq[i-3]);
            seq[i] = numerator / n_seq;                        /* division is exact */
        }
    }
    seq_len = n + 1;
    return seq[n];
}
