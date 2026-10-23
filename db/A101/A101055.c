/*
 * A101055  Expansion of e.g.f. exp(exp(x)-1)/(1-x)^3.
 * https://oeis.org/A101055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (3*(k-1)! + 1) * binomial(n-1, k-1) * a(n-k)
 *            with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* factorials: fact[i] = i!  (i >= 0) */
static seq_t *fact = NULL;
static idx_t  fact_size = 0;

/* sequence values: a_arr[i] = a(i) */
static seq_t *a_arr = NULL;
static idx_t  a_size = 0;   /* number of computed a's  (a_arr[0] always valid) */
static idx_t  a_cap  = 0;   /* allocated capacity */

/* ensure factorials up to n are available */
static void ensure_fact(idx_t n)
{
    if (fact_size <= n) {
        if (fact_size == 0) {
            fact = (seq_t*)malloc(sizeof(seq_t));
            if (!fact) return;          /* no memory – caller will fail */
            fact[0] = 1;               /* 0! = 1 */
            fact_size = 1;
        }
        while (fact_size <= n) {
            seq_t prev = fact[fact_size - 1];
            fact = (seq_t*)realloc(fact, (fact_size + 1) * sizeof(seq_t));
            if (!fact) return;          /* no memory – caller will fail */
            fact[fact_size] = prev * (seq_t)fact_size;
            fact_size++;
        }
    }
}

/* ensure a_arr[n] is computed */
static void ensure_a(idx_t n)
{
    if (a_size <= n) {
        if (a_size == 0) {
            a_cap  = 8;
            a_arr  = (seq_t*)malloc(a_cap * sizeof(seq_t));
            if (!a_arr) return;          /* no memory – caller will fail */
            a_arr[0] = 1;                 /* a(0) = 1 */
            a_size = 1;
        }

        if (n >= 1) {
            ensure_fact(n - 1);           /* factorials up to n-1 are needed */
        }

        for (idx_t m = a_size; m <= n; ++m) {
            seq_t sum   = 0;
            seq_t binom = 1;             /* C(m-1,0) */

            for (idx_t k = 1; k <= m; ++k) {
                seq_t coeff = fact[k - 1] * 3 + 1;   /* 3*(k-1)! + 1 */
                seq_t term  = coeff * binom * a_arr[m - k];
                sum += term;

                if (k < m) {
                    /* binom(m-1, k) = binom(m-1, k-1) * (m-k) / k */
                    binom = binom * (seq_t)(m - k) / (seq_t)k;
                }
            }

            if (a_size >= a_cap) {
                a_cap *= 2;
                a_arr = (seq_t*)realloc(a_arr, a_cap * sizeof(seq_t));
                if (!a_arr) return;          /* no memory – caller will fail */
            }
            a_arr[a_size] = sum;
            a_size++;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_arr[n];
}
