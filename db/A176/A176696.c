/*
 * A176696  E.g.f.: exp(6*arcsin(x))-6*arcsin(x).
 * https://oeis.org/A176696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] (exp(6*arcsin(x)) - 6*arcsin(x)) Recurrence
 *            (based on the exponential generating function): A_0 = 1 A_n =
 *            Σ_{k=1..n} C(n-1,k-1) * g_k * A_{n-k} (n ≥ 1) where g_k = k! *
 *            s_k and s_k = coefficient of x^k in 6*arcsin(x). For odd k =
 *            2t+1 : g_k = 6 * ( (2t)!^2 ) / ( t!^2 * 4^t ) For...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* for seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *A      = NULL;   /* A_n values */
    static size_t A_cap   = 0;
    static seq_t *g      = NULL;   /* g_k values (k! * s_k) */
    static size_t g_cap   = 0;
    static seq_t *fact   = NULL;   /* factorials: fact[i] = i! */
    static size_t fact_cap = 0;
    static seq_t *pow4   = NULL;   /* pow4[t] = 4^t */
    static size_t pow4_cap = 0;
    /* -------------------------------------------------------------------- */
    /* Make sure precomputed arrays are long enough */
    if (n + 1 > A_cap) {
        /* --- factorials ----------------------------------------------- */
        if (n + 1 > fact_cap) {
            size_t old = fact_cap;
            fact_cap = n + 1;
            fact = (seq_t *)realloc(fact, fact_cap * sizeof(seq_t));
            if (old == 0) { fact[0] = 1; old = 1; }
            for (size_t i = old; i < fact_cap; ++i)
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        /* --- powers of 4 ---------------------------------------------- */
        size_t t_max = n / 2;                     /* floor((n-1)/2) < t_max+1 */
        if (t_max + 1 > pow4_cap) {
            size_t old = pow4_cap;
            pow4_cap = t_max + 1;
            pow4 = (seq_t *)realloc(pow4, pow4_cap * sizeof(seq_t));
            if (old == 0) { pow4[0] = 1; old = 1; }
            for (size_t i = old; i < pow4_cap; ++i)
                pow4[i] = pow4[i - 1] * (seq_t)4;
        }
        /* --- g_k values ---------------------------------------------- */
        if (n + 1 > g_cap) {
            size_t old = g_cap;
            g_cap = n + 1;
            g = (seq_t *)realloc(g, g_cap * sizeof(seq_t));
            for (size_t i = old; i < g_cap; ++i) g[i] = 0;
            for (size_t k = old; k < g_cap; ++k) {
                if (k & 1) {                     /* odd k */
                    size_t t = (k - 1) >> 1;     /* (k-1)/2 */
                    seq_t bin  = binom((idx_t)(2 * t), (idx_t)t);      /* C(2t,t) */
                    seq_t temp = bin * fact[(idx_t)(2 * t)];           /* bin*C(2t,t) */
                    seq_t num  = temp * (seq_t)6;                     /* 6*bin*C(2t,t) */
                    g[k] = num / pow4[t];     /* guaranteed integer */
                }
            }
        }
        /* --- A_n values ----------------------------------------------- */
        if (n + 1 > A_cap) {
            size_t old = A_cap;
            A_cap = n + 1;
            A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
            for (size_t i = old; i < A_cap; ++i) A[i] = 0;
            if (old == 0) { A[0] = 1; old = 1; }   /* A[0] = 1 */
            for (size_t cur = old; cur < A_cap; ++cur) {
                seq_t sum = 0;
                seq_t binom_val = 1;                   /* C(cur-1,0) */
                for (size_t k = 1; k <= cur; ++k) {
                    if (k & 1) {                      /* g_k can be non‑zero only for odd k */
                        seq_t prod = binom_val * g[k];
                        prod = prod * A[cur - k];
                        sum += prod;
                    }
                    if (k < cur) {                    /* binom_val = binom(cur-1,k) */
                        binom_val = (binom_val * (seq_t)(cur - k)) / (seq_t)k;
                    }
                }
                A[cur] = sum;
            }
        }
    }
    /* -------------------------------------------------------------------- */
    seq_t result = A[n];
    if (n & 1)                     /* subtract g_n for odd n */
        result = result - g[n];
    return result;
}
