/*
 * A187145  McKay-Thompson series of class 12I for the Monster group with a(0)
 *          = 3.
 * https://oeis.org/A187145
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑12 sequence
 *            [3,-4,2,-2,3,-4,3,-2,2,-4,3,0] shifted by one index: a(n) =
 *            coeff of q^(n+1) in ∏_{k≥1}(1 - q^k)^(-c_k), with c_k = c[(k-1)
 *            mod 12] where c = {3,-4,2,-2,3,-4,3,-2,2,-4,3,0}
 * Range:     i8 n <= 18, i16 n <= 101, i32 n <= 395, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* External types are provided by the build environment. */
extern idx_t _floor_isqrt_u64(idx_t n); /* unused but allowed */

static seq_t *coeff = NULL;      /* coeff[i] = a_f(i) = coefficient of q^i in f(q) */
static seq_t *d_arr = NULL;      /* d_arr[k] = sum_{d|k} d * c_d */
static size_t coeff_capacity = 0;
static size_t coeff_limit = 0;    /* last computed index of coeff */

/* Ensure coeff up to target_index inclusive, replenishing d_arr as needed. */
static void ensure_up_to(idx_t target_index)
{
    if ((size_t)target_index <= coeff_limit)
        return;                               /* already computed */

    /* Expand storage capacity if required */
    size_t new_cap = coeff_capacity ? coeff_capacity : 1;
    while (new_cap <= (size_t)target_index)
        new_cap <<= 1;

    coeff = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
    d_arr   = (seq_t *)realloc(d_arr,   new_cap * sizeof(seq_t));
    /* Initialize new cells to zero */
    for (size_t i = coeff_capacity; i < new_cap; ++i) {
        coeff[i] = 0;
        d_arr[i] = 0;
    }
    coeff_capacity = new_cap;

    /* Recompute d_arr[1 .. target_index] from scratch */
    const int period = 12;
    const int c[12] = {3, -4, 2, -2, 3, -4, 3, -2, 2, -4, 3, 0};
    for (size_t i = 1; i <= (size_t)target_index; ++i)
        d_arr[i] = 0;                          /* clear for recomputation */

    for (size_t d = 1; d <= (size_t)target_index; ++d) {
        int c_val = c[(d - 1) % period];
        seq_t mult = (seq_t)d * (seq_t)c_val;
        for (size_t m = d; m <= (size_t)target_index; m += d)
            d_arr[m] += mult;
    }

    /* Compute coeff[n] for new indices */
    for (size_t n = coeff_limit + 1; n <= (size_t)target_index; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k)
            sum += d_arr[k] * coeff[n - k];
        coeff[n] = sum / (seq_t)n;            /* exact division */
    }
    coeff_limit = target_index;               /* update highest computed index */
}

/* External function required by the sequence entry. */
seq_t a(idx_t n)
{
    if (n < -1)                /* offset is -1; behavior below is unspecified */
        return (seq_t)0;

    idx_t target_index = n + 1;   /* a(n) = coeff[n+1] of f(q) */
    if (coeff_capacity == 0) {    /* first call: initialize coeff[0] = 1 */
        coeff = (seq_t *)malloc(sizeof(seq_t));
        coeff_capacity = 1;
        coeff[0] = (seq_t)1;      /* product at q^0 is 1 */
        coeff_limit = 0;
        d_arr = (seq_t *)malloc(sizeof(seq_t));
        d_arr[0] = 0;             /* unused */
    }
    ensure_up_to(target_index);
    return coeff[target_index];
}
