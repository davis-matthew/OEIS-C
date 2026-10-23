/*
 * A164268  Expansion of f(q^3) * phi(q^3)^3 / (q * f(q^9)^3 * phi(q)) in
 *          powers of q where f(), phi() are Ramanujan theta functions.
 * https://oeis.org/A164268
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Euler transform of the period‑36 sequence * b = [-2, 3,
 *            5, 1, -2, -8, -2, 1, 2, 3, -2, -3, -2, 3, 5, 1, * -2, -2, -2, 1,
 *            5, 3, -2, -3, -2, 3, 2, 1, -2, -8, -2, 1, * 5, 3, -2, 0] * Let
 *            e(k) = -b[(k-1) mod 36] for k ≥ 1. Define * c(0) = 1 * c(n) =
 *            -(1/n) * Σ_{k=1}^{n} c(n‑k) * Σ_{d|k} d·e(d)...
 * Range:     i8 n <= 18, i16 n <= 120, i32 n <= 419, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- packed period‑36 sequence --------------------------------------- */
static const int b[36] = {
    -2,  3,  5,  1, -2, -8, -2,  1,  2,  3, -2, -3,
    -2,  3,  5,  1, -2, -2, -2,  1,  5,  3, -2, -3,
    -2,  3,  2,  1, -2, -8, -2,  1,  5,  3, -2,  0
};

/* ----- memoised coefficients c(0..k)  --------------------------------- */
static seq_t *c = NULL;          /* c[0] … c[needed_index] */
static size_t c_size = 0;        /* number of stored elements (>0)  */

/* ``recompute_c`` calculates c[0..need] from scratch.                   */
static void recompute_c(size_t need)
{
    /* arrays are 0‑based; element 0 is unused for e and divsum          */
    size_t sz = need + 1;                          /* indices 0..need      */

    seq_t *e      = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *divsum = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *new_c  = (seq_t*)malloc(sz * sizeof(seq_t));

    /* e(k) = -b[(k-1) mod 36]                                       */
    for (size_t k = 1; k <= need; ++k)
        e[k] = -(seq_t)b[(k - 1) % 36];

    /* divisor sums: divsum[k] = Σ_{d|k} d * e(d)                      */
    for (size_t d = 1; d <= need; ++d) {
        if (e[d] == (seq_t)0) continue;
        seq_t val = e[d] * (seq_t)d;
        for (size_t m = d; m <= need; m += d)
            divsum[m] += val;
    }

    /* recurrence for c */
    new_c[0] = (seq_t)1;
    for (size_t n = 1; n <= need; ++n) {
        seq_t s = (seq_t)0;
        for (size_t k = 1; k <= n; ++k)
            s += divsum[k] * new_c[n - k];
        new_c[n] = -(s / (seq_t)n);
    }

    free(e);
    free(divsum);

    /* replace old array */
    if (c) free(c);
    c = new_c;
    c_size = sz;            /* new_c[0..need] are valid                */
}

/* ``ensure_c_upto`` guarantees that c[0..need] exists.                    */
static void ensure_c_upto(idx_t n)
{
    if (n < -1) return;             /* values below the offset irrelevant  */
    size_t need = (size_t)(n + 1);   /* we need c[n+1]                       */
    if (c_size > need) return;
    recompute_c(need);
}

/* ---- public function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == -1) return (seq_t)1;   /* a(-1)=1                               */
    if (n < -1)  return (seq_t)0;   /* below offset – unspecified            */

    ensure_c_upto(n);
    return c[(size_t)(n + 1)];      /* a(n)=c[n+1] for n ≥ 0                */
}
