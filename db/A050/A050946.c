/*
 * A050946  "Stirling-Bernoulli transform" of Fibonacci numbers.
 * https://oeis.org/A050946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^{n-k} S2(n,k) * k! * Fibonacci(k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where S2 are Stirling numbers of the second kind and Fibonacci(0)=0, Fibonacci(1)=1. */

#include <stdlib.h>

/* helpers from utils (the build supplies seq_t/idx_t, plus these allocation helpers) */
#include "../utils/seq_realloc.h"      /* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* static state to memoise computed values */
static idx_t  max_n     = 0;     /* largest n for which values are already computed */
static size_t capacity  = 0;     /* current allocation capacity of the arrays */
static seq_t *S          = NULL; /* Stirling numbers S2(n,k) for current n  (shared array) */
static seq_t *fact       = NULL; /* factorials k! up to current n */
static seq_t *fibarr     = NULL; /* Fibonacci numbers up to current n */
static seq_t *aarr       = NULL; /* already computed a(n) values */

/* allocate or grow all needed arrays to at least new_cap elements */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= (idx_t)capacity)
        return;

    /* use the provided reallocation helper which works for both C and C++ builds */
    S      = seq_realloc(S,   capacity, (size_t)new_cap);
    fact   = seq_realloc(fact, capacity, (size_t)new_cap);
    fibarr = seq_realloc(fibarr, capacity, (size_t)new_cap);
    aarr   = seq_realloc(aarr, capacity, (size_t)new_cap);

    /* initialise the newly allocated entries to zero */
    for (idx_t i = (idx_t)capacity; i < new_cap; ++i) {
        S[i] = 0;
        fact[i] = 0;
        fibarr[i] = 0;
        aarr[i] = 0;
    }
    capacity = (size_t)new_cap;
}

seq_t a(idx_t n)
{
    /* initialise memoisation state on first call */
    if (max_n == 0 && S == NULL) {
        ensure_capacity(1);         /* need room for index 0 */
        S[0]      = 1;              /* S2(0,0) = 1 */
        fact[0]   = 1;              /* 0! = 1 */
        fibarr[0] = 0;              /* Fibonacci(0) = 0 */
        aarr[0]   = 0;              /* a(0) = 0 */
        max_n = 0;
    }

    /* compute up to desired n if necessary */
    if (n > max_n) {
        ensure_capacity(n + 1);     /* we need indices 0..n */

        /* loop from current maximum up to n, computing each new value */
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* ---------- factorial ---------- */
            fact[cur] = fact[cur - 1] * (seq_t)cur;

            /* ---------- Fibonacci ---------- */
            if (cur == 1) {
                fibarr[1] = 1;
            } else {
                fibarr[cur] = fibarr[cur - 1] + fibarr[cur - 2];
            }

            /* ---------- Stirling S2(n,k) ---------- */
            /* Build in place using the recurrence:
               S(n,k) = k * S(n-1,k) + S(n-1,k-1)  (for k >= 1)
               S(n,0) = 0 for n>0
            */
            S[cur] = 0;                     /* new element initially 0 */
            for (idx_t k = cur; k >= 1; --k) {
                /* old S[k] is still the value from n-1 because we iterate descending */
                seq_t term1 = S[k];          /* k * S(n-1,k) will use old S[k] */
                term1 = term1 * (seq_t)k;    /* multiply by k */
                seq_t term2 = (k > 0 ? S[k - 1] : 0);  /* S(n-1,k-1) old value */
                S[k] = term1 + term2;        /* new Stirling value */
            }
            if (cur > 0) S[0] = 0;           /* S(>0,0) = 0 */

            /* ---------- compute a(cur) ---------- */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (idx_t k = 1; k <= cur; ++k) {   /* k=0 contributes zero */
                /* unsigned term = S[cur][k] * k! * Fibonacci(k) */
                seq_t term = S[k] * fact[k];
                term = term * fibarr[k];

                /* decide sign by parity of (cur - k) */
                if (((cur - k) & (idx_t)1) == 0) {
                    sum_pos += term;           /* even difference → positive */
                } else {
                    sum_neg += term;           /* odd difference  → negative */
                }
            }
            /* a(cur) = sum_pos - sum_neg (sum_pos > sum_neg for all n>0) */
            aarr[cur] = sum_pos - sum_neg;
        }
        max_n = n;
    }

    return aarr[n];
}
