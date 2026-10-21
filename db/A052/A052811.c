/*
 * A052811  A simple grammar: sequences of pairs of cycles.
 * https://oeis.org/A052811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * Σ_{k=1..n} C(n,k) * |S1(k,2)| * a(n-k), a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(n,k) = binomial coefficient. |S1(k,2)| = (k-1)! * H_{k-1}  (integer). */
/* This implementation builds factorials, the |S1(k,2)| table and the sequence
   values lazily up to the requested n. All operations are performed using
   the seq_t type as required. */

#include <stdlib.h>
#include "../utils/grow.h"

/* Forward declarations */
static void ensure_fact(idx_t n);
static void ensure_c(idx_t n);
static void ensure_a(idx_t n);

/* Dynamic arrays */
static seq_t *fact = NULL;   /* factorials fact[i] = i!  */
static seq_t *c    = NULL;   /* c[k]   = |S1(k,2)|   */
static seq_t *A    = NULL;   /* A[n]   = a(n)        */

static idx_t fact_len = 0, fact_cap = 0;
static idx_t c_len    = 0, c_cap    = 0;
static idx_t A_len    = 0, A_cap    = 0;

/* Ensure factorials up to n are available */
static void ensure_fact(idx_t n)
{
    if (fact_len > n) return;
    fact = (seq_t *)grow(fact, &fact_cap, n + 1, sizeof(seq_t));
    while (fact_len <= n) {
        if (fact_len == 0) {
            fact[0] = (seq_t)1;          /* 0! */
        } else {
            fact[fact_len] = fact[fact_len - 1] * (seq_t)fact_len;
        }
        ++fact_len;
    }
}

/* Ensure |S1(k,2)| values up to k = n are available.
   Uses the recurrence: c[2] = 1;
                         c[k] = (k-1)*c[k-1] + (k-2)!  for k >= 3
 */
static void ensure_c(idx_t n)
{
    if (c_len > n) return;
    c = (seq_t *)grow(c, &c_cap, n + 1, sizeof(seq_t));
    while (c_len <= n) {
        if (c_len == 0 || c_len == 1) {
            c[c_len] = (seq_t)0;
        } else if (c_len == 2) {
            c[2] = (seq_t)1;
        } else {
            seq_t term = (seq_t)(c_len - 1) * c[c_len - 1];
            term = term + fact[c_len - 2];
            c[c_len] = term;
        }
        ++c_len;
    }
}

/* Ensure sequence values up to n are available using the recurrence above */
static void ensure_a(idx_t n)
{
    if (A_len > n) return;
    A = (seq_t *)grow(A, &A_cap, n + 1, sizeof(seq_t));
    while (A_len <= n) {
        if (A_len == 0) {
            A[0] = (seq_t)1;
        } else if (A_len == 1) {
            A[1] = (seq_t)0;
        } else {
            idx_t m = A_len;
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;      /* C(m,0) */
            for (idx_t k = 1; k <= m; ++k) {
                binom = binom * (seq_t)(m - k + 1);
                binom = binom / (seq_t)k;    /* exact division */
                seq_t mul = binom * c[k];
                mul = mul * A[m - k];
                sum = sum + mul;
            }
            A[m] = sum * (seq_t)2;
        }
        ++A_len;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Initialise arrays on first call */
    if (A == NULL) {
        A_cap = 4;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        A_len = 0;
    }
    if (c == NULL) {
        c_cap = 4;
        c = (seq_t *)malloc(c_cap * sizeof(seq_t));
        c_len = 0;
    }
    if (fact == NULL) {
        fact_cap = 4;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact_len = 0;
    }

    /* Build tables up to n */
    ensure_fact(n);   /* factorials needed for c[n]  */
    ensure_c(n);      /* |S1(k,2)| up to k = n    */
    ensure_a(n);      /* sequence values up to n   */

    return A[n];
}
