/*
 * A104460  Number of hierarchical orderings for n unlabeled elements with 2
 *          possible classes for levels l>=2.
 * https://oeis.org/A104460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1 a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d *
 *            3^(d-1) ) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Euler transform of the sequence [1,3,9,27,…]  */

#include <stdlib.h>
#include <stdint.h>

static idx_t max_n = 0;            /* largest n for which a(n) is stored   */
static size_t capacity = 0;        /* allocated array length (>= max_n+1) */
static seq_t *a_arr = NULL;       /* a_arr[i] = a(i)                       */
static seq_t *c_arr = NULL;       /* c_arr[i] = Σ_{d|i} d*3^(d-1)           */

/* Ensure internal arrays can address indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;

    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    a_arr = tmp;
    tmp = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
    c_arr = tmp;

    /* Initialise new entries to 0.  Index 0 is unused for the sequence
       but we set it anyway for safety. */
    for (idx_t i = capacity; i < new_cap; ++i) {
        a_arr[i] = (seq_t)0;
        c_arr[i] = (seq_t)0;
    }
    capacity = new_cap;
}

/* Recompute the auxiliary array c_arr[1..n] = Σ_{d|k} d*3^(d-1). */
static void recompute_c_arr(idx_t n)
{
    /* zero existing values */
    for (idx_t i = 1; i <= n; ++i)
        c_arr[i] = (seq_t)0;

    seq_t pow3 = (seq_t)1;          /* pow3 = 3^(d-1)  for current d */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t contrib = (seq_t)d * pow3;
        for (idx_t m = d; m <= n; m += d) {
            c_arr[m] += contrib;
        }
        pow3 *= (seq_t)3;           /* next power of 3 */
    }
}

/* Compute a(k) for all k from max_n+1 up to n using the recurrence. */
static void compute_a_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    ensure_capacity(n);
    recompute_c_arr(n);

    a_arr[0] = (seq_t)1;           /* define a(0) for recurrence */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t t = 1; t <= k; ++t) {
            sum += c_arr[t] * a_arr[k - t];
        }
        a_arr[k] = sum / (seq_t)k; /* division is exact by construction */
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) is not part of the sequence but harmless */
    compute_a_up_to(n);
    return a_arr[n];
}
