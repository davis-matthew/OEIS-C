/*
 * A081066  Even order Taylor expansion coefficients at x=0 of
 *          exp(exp(x^2/2)-1), odd order coefficients being equal to zero.
 * https://oeis.org/A081066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2*n-1)!! * Bell(n)   (a(0)=1)
 *
 * Bell(n) is the nth Bell number.
 * (2*n-1)!! is the odd double factorial.
 * The implementation uses dynamic arrays allocated with malloc/realloc
 * and recomputes the needed values on demand.
 */

#include <stdlib.h>
#include <stddef.h>

/*--- Cached dynamic arrays ----------------------------------------*/

static seq_t *bell      = NULL;
static size_t bell_len  = 0;
static size_t bell_cap  = 0;

static seq_t *dfact     = NULL;
static size_t dfact_len = 0;
static size_t dfact_cap = 0;

/* Ensure that bell has capacity for at least new_cap elements */
static void ensure_bell_capacity(size_t new_cap)
{
    if (new_cap <= bell_cap) return;
    size_t new_size = new_cap;
    seq_t *new_arr = NULL;
    if (bell == NULL) {
        new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else {
        new_arr = (seq_t*)realloc(bell, new_size * sizeof(seq_t));
    }
    if (new_arr == NULL) return;          /* allocation failed – give up */
    bell = new_arr;
    bell_cap = new_size;
    if (bell_len == 0) {                   /* first time initialisation */
        bell[0] = (seq_t)1;
        bell_len = 1;
    }
}

/* Ensure that dfact has capacity for at least new_cap elements */
static void ensure_dfact_capacity(size_t new_cap)
{
    if (new_cap <= dfact_cap) return;
    size_t new_size = new_cap;
    seq_t *new_arr = NULL;
    if (dfact == NULL) {
        new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else {
        new_arr = (seq_t*)realloc(dfact, new_size * sizeof(seq_t));
    }
    if (new_arr == NULL) return;          /* allocation failed – give up */
    dfact = new_arr;
    dfact_cap = new_size;
    if (dfact_len == 0) {                   /* first time initialisation */
        dfact[0] = (seq_t)1;
        dfact_len = 1;
    }
}

/*--- Main sequence function -------------------------------------*/

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* n is non‑negative uint64_t */

    /* Ensure we have Bell numbers up to idx */
    if (bell_len <= idx) {
        ensure_bell_capacity(idx + 1);
        for (size_t m = bell_len; m <= idx; ++m) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;               /* C(m-1,0) */
            for (size_t k = 0; k < m; ++k) {     /* k = 0 .. m-1 */
                sum += binom * bell[k];
                /* Update binom to C(m-1,k+1) */
                seq_t mult = (seq_t)(m - 1 - k);
                seq_t div  = (seq_t)(k + 1);
                binom = (binom * mult) / div;
            }
            bell[m] = sum;
        }
    }

    /* Ensure we have double factorials up to idx */
    if (dfact_len <= idx) {
        ensure_dfact_capacity(idx + 1);
        for (size_t m = dfact_len; m <= idx; ++m) { /* m = 1 .. idx */
            seq_t factor = (seq_t)(2 * m - 1);
            dfact[m] = dfact[m - 1] * factor;
        }
    }

    return dfact[idx] * bell[idx];
}
