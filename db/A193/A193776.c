/*
 * A193776  Number of signed permutations of length n invariant under the
 *          reverse complement and avoiding (-2, -1), (-2, 1), (2, -1).
 * https://oeis.org/A193776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193776: Number of signed permutations of length n invariant under the
 * reverse complement and avoiding (-2, -1), (-2, 1), (2, -1).
 * Recurrence:
 *   a(0) = 1, a(1) = 2
 *   a(2k)   = 2^k k! + Σ_{j=0}^{k-1} (k-j-1)! a(2j)
 *   a(2k+1) = (2^k + 1) k! + Σ_{j=0}^{k-1} (k-j-1)! a(2j+1)   */
#include <stdlib.h>
#include <stddef.h>

static seq_t *even_arr = NULL;     /* a(2k) */
static seq_t *odd_arr  = NULL;     /* a(2k+1) */
static seq_t *fact_arr = NULL;     /* k!   */
static seq_t *pow2_arr = NULL;     /* 2^k  */
static idx_t  cap     = 0;         /* allocated size */
static idx_t  cur_k   = 0;         /* largest k computed */

/* Ensure arrays can hold indices up to 'k_req'.  Grow by doubling. */
static void ensure_capacity(idx_t k_req)
{
    if (k_req < cap)
        return;
    idx_t new_cap = (cap == 0) ? 1 : cap;
    while (new_cap <= k_req)
        new_cap *= 2;

    even_arr = (seq_t *)realloc(even_arr, sizeof(seq_t) * new_cap);
    odd_arr  = (seq_t *)realloc(odd_arr,  sizeof(seq_t) * new_cap);
    fact_arr = (seq_t *)realloc(fact_arr, sizeof(seq_t) * new_cap);
    pow2_arr = (seq_t *)realloc(pow2_arr, sizeof(seq_t) * new_cap);

    /* Initialize newly allocated entries to 0; they will be set later. */
    for (idx_t i = cap; i < new_cap; ++i) {
        fact_arr[i] = 0;
        pow2_arr[i] = 0;
    }

    cap = new_cap;
}

/* One-time initialization of base cases (k = 0). */
static void init_base_case(void)
{
    if (even_arr != NULL)
        return; /* already initialized */

    ensure_capacity(0);
    fact_arr[0] = (seq_t)1;
    pow2_arr[0] = (seq_t)1;
    even_arr[0] = (seq_t)1; /* a(0) */
    odd_arr[0]  = (seq_t)2; /* a(1) */
    cur_k = 0;
}

seq_t a(idx_t n)
{
    init_base_case();

    /* Determine the required k. */
    idx_t k = (n % 2 == 0) ? n / 2 : (n - 1) / 2;

    /* Compute up to k if not yet done. */
    for (idx_t next_k = cur_k + 1; next_k <= k; ++next_k) {
        ensure_capacity(next_k);

        /* factorial[next_k] = factorial[next_k-1] * next_k */
        fact_arr[next_k] = fact_arr[next_k - 1] * (seq_t)next_k;

        /* pow2[next_k] = 2 * pow2[next_k-1] */
        pow2_arr[next_k] = pow2_arr[next_k - 1] * (seq_t)2;

        seq_t sum_even = (seq_t)0;
        seq_t sum_odd  = (seq_t)0;
        for (idx_t j = 0; j < next_k; ++j) {
            seq_t mul = fact_arr[next_k - j - 1];
            sum_even += mul * even_arr[j];
            sum_odd  += mul * odd_arr[j];
        }

        seq_t base_even = pow2_arr[next_k] * fact_arr[next_k];
        even_arr[next_k] = base_even + sum_even;

        seq_t base_odd  = (pow2_arr[next_k] + (seq_t)1) * fact_arr[next_k];
        odd_arr[next_k]  = base_odd + sum_odd;
    }
    cur_k = k;

    return (n % 2 == 0) ? even_arr[k] : odd_arr[k];
}
