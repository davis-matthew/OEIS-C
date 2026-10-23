/*
 * A009623  Expansion of e.g.f. sinh(x)*exp(sinh(x)).
 * https://oeis.org/A009623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A009623 – expansion of e.g.f. sinh(x)*exp(sinh(x))
 *
 *  The coefficients a(n) satisfy the following relations:
 *
 *      e_0 = 1
 *      e_n = Σ_{k odd ≤ n}  C(n-1, k-1) * e_{n-k}          (n ≥ 1)
 *
 *      a_0 = 0
 *      a_n = Σ_{k odd ≤ n}  C(n, k) * e_{n-k}               (n ≥ 1)
 *
 *  where C(m, r) is the binomial coefficient “m choose r”.
 *  The arrays e[ ] and a[ ] are memoised so that calls with arbitrary
 *  (possibly decreasing) indices are handled efficiently.
 *
 *  The implementation uses only integer arithmetic.  The type `seq_t`
 *  can be an arbitrary‑precision integer; the same code works for
 *  8/16/32/64‑bit unsigned integers because all intermediate values
 *  are integers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"

static seq_t *e_arr   = NULL;   /* e_n = coefficient of e^sinh(x) */
static seq_t *a_arr   = NULL;   /* desired sequence a(n)           */
static idx_t  size_allocated = 0;   /* number of slots allocated     */
static idx_t  computed       = 0;   /* largest n that has been computed */

/* Ensure we have slots for indices 0 … n, initialise new slots. */
static void
ensure_alloc(idx_t n)
{
    if (n < size_allocated)
        return;
    idx_t old_size = size_allocated;
    size_allocated = n + 1;
    e_arr = (seq_t *)realloc(e_arr, size_allocated * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, size_allocated * sizeof(seq_t));
    assert(e_arr && a_arr);
    /* Initialise new slots to zero. */
    for (idx_t i = old_size; i <= n; ++i) {
        e_arr[i] = 0;
        a_arr[i] = 0;
    }
    /* Base case initialised */
    e_arr[0] = 1;
    a_arr[0] = 0;
}

/* Compute e[ ] and a[ ] for all n from (computed+1) up to n. */
static void
compute_up_to(idx_t n)
{
    ensure_alloc(n);
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* -------- compute e[i] -------- */
        seq_t sum_e = 0;
        seq_t binom = 1;                     /* C(i-1, 0) */
        for (idx_t k = 1; k <= i; k += 2) {  /* k odd */
            sum_e += binom * e_arr[i - k];
            if (k + 2 <= i) {                /* update for next odd k */
                seq_t num1 = (seq_t)(i - k);      /* i-1-(k-1) */
                seq_t num2 = (seq_t)(i - k - 1);  /* i-1-(k-1)-1 */
                seq_t den1 = (seq_t)(k);          /* k   = (k-1)+1 */
                seq_t den2 = (seq_t)(k + 1);      /* k+1 = (k-1)+2 */
                binom = binom * num1 * num2 / (den1 * den2);
            }
        }
        e_arr[i] = sum_e;

        /* -------- compute a[i] -------- */
        seq_t sum_a = 0;
        seq_t binom_a = (seq_t)i;      /* C(i, 1) */
        for (idx_t k = 1; k <= i; k += 2) {   /* k odd */
            sum_a += binom_a * e_arr[i - k];
            if (k + 2 <= i) {                  /* update for next odd k */
                seq_t num1 = (seq_t)(i - k);
                seq_t num2 = (seq_t)(i - k - 1);
                seq_t den1 = (seq_t)(k + 1);
                seq_t den2 = (seq_t)(k + 2);
                binom_a = binom_a * num1 * num2 / (den1 * den2);
            }
        }
        a_arr[i] = sum_a;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    compute_up_to(n);
    return a_arr[n];
}
