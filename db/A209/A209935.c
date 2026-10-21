/*
 * A209935  G.f.: 1 = Sum_{n>=0} a(n)*x^n*Product_{k=1..n+1} (1-k^2*x) for n>0
 *          with a(0)=1.
 * https://oeis.org/A209935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the recurrence derived from the generating function
   1 = Σ_{n≥0} a(n)·xⁿ·∏_{k=1}^{n+1}(1–k²·x) .
   For every n ≥ 0 it returns a(n).  The values are built incrementally
   and cached, so calls may be in any order. */

#include <stdlib.h>
#include <stdint.h>

/* The following static variables keep the cached sequence
   and auxiliary convolution data.  They are initially
   zero‑filled and expanded as needed during the first call. */
static seq_t *a_cache  = NULL;
static seq_t *pos     = NULL;
static seq_t *neg     = NULL;
static seq_t *e       = NULL;
static idx_t computed_up_to = (idx_t)-1;   /* no values computed yet */
static idx_t e_len          = 0;           /* largest r index in e[] currently set */

/* For the intended use (n up to about 10 000) a fixed size is sufficient.
   2 · n + 2 bounds the maximum index that can be reached (d = n + r, r≤n+1). */
#define MAX_N   10000U
#define CAP     (2U * MAX_N + 2)

/* Initialise all static arrays and the first term a(0). */
static void init_arrays(void)
{
    a_cache = (seq_t*)calloc(CAP, sizeof(seq_t));
    pos     = (seq_t*)calloc(CAP, sizeof(seq_t));
    neg     = (seq_t*)calloc(CAP, sizeof(seq_t));
    e       = (seq_t*)calloc(CAP, sizeof(seq_t));

    /* e[0] = 1, e[1] = 1 corresponds to the product (1–1²x). */
    e[0] = (seq_t)1;
    e[1] = (seq_t)1;
    e_len = 1;

    /* a(0) = 1 */
    a_cache[0] = (seq_t)1;
    pos[0] += a_cache[0] * e[0];          /* even r → positive coefficient */
    neg[1] += a_cache[0] * e[1];          /* odd  r → negative coefficient */

    computed_up_to = 0;
}

/* Return a(n) for n ≥ 0.  Calls may be in any order. */
seq_t a(idx_t n)
{
    if (a_cache == NULL)            /* first call */
        init_arrays();

    /* Compute missing terms up to n */
    if (n > computed_up_to) {
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            /* Update e[] to include the new factor (1–(k+1)²x). */
            seq_t sq = (seq_t)(k + 1) * (seq_t)(k + 1);
            if (e_len + 1 < CAP)
                e[e_len + 1] = (seq_t)0;   /* initialise the new slot */

            for (idx_t r = e_len + 1; r > 0; --r)
                e[r] += sq * e[r - 1];

            ++e_len;   /* now e contains indices 0 .. e_len */

            /* Determine a(k) from the current coefficient at degree k. */
            seq_t a_k = neg[k];
            a_k -= pos[k];
            a_cache[k] = a_k;

            /* Add the contribution of a(k) to the coefficient array. */
            for (idx_t r = 0; r <= e_len; ++r) {
                idx_t d = k + r;
                if (d >= CAP) break;      /* safety guard */
                seq_t prod = a_k * e[r];
                if ((r & 1) == 0)
                    pos[d] += prod;        /* even r → + coefficient */
                else
                    neg[d] += prod;        /* odd r  → - coefficient */
            }
        }
        computed_up_to = n;
    }
    return a_cache[n];
}
