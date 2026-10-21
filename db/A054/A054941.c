/*
 * A054941  Number of weakly connected oriented graphs on n labeled nodes.
 * https://oeis.org/A054941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n) - ∑_{k=1}^{n-1} C(n-1,k-1) · a(k) · T(n-k), where
 *            T(n) = 3^{ n·(n-1)/2 } is the total number of oriented graphs on
 *            n labeled vertices. a(1)=1. This recursive formula follows from
 *            the logarithm of the egf: log( Σ_{n≥0} 3^{C(n,2)} x^n/n! )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of function as required by the specification */
seq_t a(idx_t n);

/* Internal static storage ---------------------------------------------------- */

static idx_t   computed_max = 0;      /* largest n that has been computed   */
static size_t  capacity     = 0;      /* allocated size of the arrays       */
static seq_t *T            = NULL;    /* T[n] = 3^{n·(n-1)/2}              */
static seq_t *a_table      = NULL;    /* a[n] values                        */
static seq_t pow3          = 1;       /* 3^{computed_max}  (updated on each step) */

/* Ensure that values up to n are available, expanding storage as necessary.  */
static void ensure_up_to(idx_t n) {
    if (n <= computed_max) return;

    /* Allocate or re‑allocate arrays as needed.  */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > capacity) {
        T        = (seq_t *)realloc(T,        new_cap * sizeof(seq_t));
        a_table  = (seq_t *)realloc(a_table,  new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Minor initialization if we are starting from scratch.  */
    if (computed_max == 0) {
        T[0] = 1;           /* 3^{C(0,2)}  = 1 */
        a_table[0] = 0;      /* unused */
        pow3      = 1;        /* 3^{0}      = 1 */
    }

    /* Compute all missing values.  */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* T[i] = T[i-1] * 3^{i-1}.  pow3 holds 3^{i-1} at this point. */
        T[i] = T[i-1] * pow3;
        pow3 = pow3 * 3;           /* prepare pow3 for next i (3^i) */

        /* Compute a[i] using the recurrence.  */
        seq_t sum = 0;            /* accumulating ∑ C · a[k] · T[n-k]  */
        seq_t C   = 1;            /* C = binom(i-1, 0)  */

        for (idx_t k = 1; k <= i - 1; ++k) {
            seq_t term = C * a_table[k];
            term = term * T[i - k];
            sum = sum + term;

            /* Update C to binom(i-1, k) for next iteration.  */
            if (k < i - 1) {
                C = C * (seq_t)(i - k);   /* multiply by (i-k) */
                C = C / (seq_t)k;         /* divide by k   */
            }
        }
        a_table[i] = T[i] - sum;
    }

    computed_max = n;
}

/* --------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for n=0, but harmless */
    ensure_up_to(n);
    return a_table[n];
}
