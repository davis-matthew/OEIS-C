/*
 * A013218  E.g.f.: arctanh(log(x+1)-sin(x)) = -1/2!*x^2 + 3/3!*x^3 - 6/4!*x^4
 *          + 23/5!*x^5 + ...
 * https://oeis.org/A013218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] arctanh(log(1+x)-sin(x)) as an exponential
 *            generating function
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *B = NULL;      /* coefficients of f(x)=log(1+x)-sin(x) in EGF order */
static seq_t *E = NULL;      /* coefficients of C(x)=1-f(x)^2 */
static seq_t *H = NULL;      /* coefficients of h'(x) = a_{n+1} in EGF order */
static seq_t *fact = NULL;   /* factorials */
static size_t cap = 0;       /* capacity of B, E, fact arrays             */
static size_t h_cap = 0;     /* capacity of H array                          */

/* Ensure that arrays are allocated up to index 'n' (inclusive) */
static void ensure_size(idx_t n)
{
    /* We need B up to n+1 and H up to n-1, so allocate at least n+2 elements for B/E/fact */
    size_t need = (size_t)n + 2;
    if (need <= cap) return; /* already enough */

    size_t old_cap = cap;
    size_t new_cap = need;

    /* Reallocate B, E, and fact */
    B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    E = (seq_t *)realloc(E, new_cap * sizeof(seq_t));
    fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    cap = new_cap;

    /* Compute new factorials and B coefficients */
    for (size_t i = old_cap; i < cap; ++i) {
        if (i == 0) {
            fact[0] = 1;
            B[0] = 0;          /* f(0)=0          */
            continue;
        }
        fact[i] = fact[i - 1] * (seq_t)i;   /* factorial */
        /* L = (-1)^(i-1) * (i-1)! */
        seq_t L_sign = ((i - 1) & 1) ? -1 : 1;
        seq_t L = L_sign * fact[i - 1];
        /* Subtract sin part if i is odd */
        seq_t Bval = L;
        if (i & 1) {
            seq_t sgn = (((i - 1) / 2) & 1) ? -1 : 1;  /* (-1)^((i-1)/2) */
            Bval -= sgn;   /* f = log - sin */
        }
        B[i] = Bval;
    }

    /* Compute E coefficients: C(x) = 1 - f(x)^2 */
    E[0] = 1;   /* C(0)=1 */
    for (size_t n = (old_cap > 1 ? old_cap : 1); n < cap; ++n) {
        seq_t P = 0;
        seq_t binom = 1;          /* binom(n,0) */
        for (size_t i = 0; i <= n; ++i) {
            seq_t term = binom * B[i] * B[n - i];
            P += term;
            if (i == n) break;
            binom = binom * (seq_t)(n - i) / (seq_t)(i + 1);  /* binom(n,i+1) */
        }
        E[n] = -P;   /* E[n] = -P for n>=1 */
    }

    /* Reallocate H array (size cap-1) */
    size_t old_h_cap = h_cap;
    h_cap = cap - 1;
    H = (seq_t *)realloc(H, h_cap * sizeof(seq_t));

    /* Compute H coefficients: h' = f' / (1 - f^2) */
    /* H[0] = D[0] = B[1] / E[0] (E[0]==1) */
    if (old_h_cap == 0) {
        H[0] = B[1];  /* B[1] is D[0] */
    }
    for (size_t n = old_h_cap; n < h_cap; ++n) {
        seq_t sum = 0;
        seq_t binom = 1;  /* binom(n,0) */
        for (size_t i = 1; i <= n; ++i) {
            binom = binom * (seq_t)(n - (i - 1)) / (seq_t)i;  /* binom(n,i) */
            sum += binom * E[i] * H[n - i];
        }
        H[n] = B[n + 1] - sum;   /* D[n] = B[n+1] */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) = 0 */
    ensure_size((idx_t)n);
    return H[(size_t)n - 1];       /* a(n) = H[n-1] for n>=1 */
}
