/*
 * A159324  n! times the average number of comparisons required by an
 *          insertion sort of n (distinct) elements.
 * https://oeis.org/A159324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*n + n!*(n+1)/2 - (n-1)! a(0) = 0, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers required by both C and C++ */
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Sequence value type defined by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* C++ implementation: use std::vector to store values and factorials */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus

/* static storage for values and factorials */
static std::vector<seq_t> a_vals = {0, 0};
static std::vector<seq_t> fac_vals = {1, 1};
static idx_t memo_max = 1;   /* largest n already computed */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* grow storage if necessary */
    if (n >= a_vals.size()) {
        a_vals.resize(n + 1);
        fac_vals.resize(n + 1);
    }

    /* compute missing terms */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        /* factorial i! = (i-1)! * i */
        fac_vals[i] = fac_vals[i - 1] * (seq_t)i;

        /* term1 = fac_i * (i+1) / 2 */
        seq_t term1 = fac_vals[i] * (seq_t)(i + 1) / (seq_t)2;

        /* term2 = fac_i / i = (i-1)! */
        seq_t term2 = fac_vals[i] / (seq_t)i;

        /* recurrence: a(i) = a(i-1)*i + term1 - term2 */
        a_vals[i] = a_vals[i - 1] * (seq_t)i + term1 - term2;
    }

    memo_max = (idx_t) n;
    return a_vals[n];
}

#else /* ---------------------------------------------------------- */
/* C implementation: use malloc/realloc for dynamic arrays */
/* ------------------------------------------------------------ */

static seq_t *a_vals  = NULL;   /* array of a(n) */
static seq_t *fac_vals = NULL; /* array of n! */
static idx_t  cap = 0;         /* current allocated size */
static idx_t  memo_max = 1;    /* largest n already computed */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (cap == 0) {
        cap = 2;                     /* store indices 0 and 1 */
        a_vals   = (seq_t*)malloc(cap * sizeof(seq_t));
        fac_vals = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!a_vals || !fac_vals) { /* allocation failure: give up */
            if (a_vals) free(a_vals);
            if (fac_vals) free(fac_vals);
            cap = memo_max = 0;
            return 0;
        }
        a_vals[0] = 0;
        a_vals[1] = 0;
        fac_vals[0] = 1;
        fac_vals[1] = 1;
    }

    /* grow arrays if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *a_new = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        seq_t *f_new = (seq_t*)realloc(fac_vals, new_cap * sizeof(seq_t));
        if (!a_new || !f_new) { /* allocation failure: free everything */
            if (a_new) free(a_new);
            if (f_new) free(f_new);
            free(a_vals);
            free(fac_vals);
            cap = memo_max = 0;
            return 0;
        }
        a_vals = a_new;
        fac_vals = f_new;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        /* factorial i! = (i-1)! * i */
        fac_vals[i] = fac_vals[i - 1] * (seq_t)i;

        /* term1 = fac_i * (i+1) / 2 */
        seq_t term1 = fac_vals[i] * (seq_t)(i + 1) / (seq_t)2;

        /* term2 = fac_i / i = (i-1)! */
        seq_t term2 = fac_vals[i] / (seq_t)i;

        /* recurrence: a(i) = a(i-1)*i + term1 - term2 */
        a_vals[i] = a_vals[i - 1] * (seq_t)i + term1 - term2;
    }

    memo_max = n;
    return a_vals[n];
}

#endif /* end of C / C++ conditional */
