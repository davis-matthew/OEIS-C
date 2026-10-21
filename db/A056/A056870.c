/*
 * A056870  Difference between partial sums of partition numbers (A026905) and
 *          partial sums of numbers of partitions into distinct parts
 *          (A026906).
 * https://oeis.org/A056870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} p(k) – ∑_{k=1..n} q(k) where p(k) =
 *            unrestricted partitions of k (A026905) q(k) = partitions of k
 *            into distinct parts (A026906)
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for memoisation */
static seq_t *p_arr = NULL;   /* unrestricted partitions, p(k)   */
static seq_t *q_arr = NULL;   /* distinct‑part partitions, q(k)  */
static seq_t *sp_arr = NULL;  /* partial sums of p(k)            */
static seq_t *sd_arr = NULL;  /* partial sums of q(k)            */
static idx_t  max_done = 0;   /* highest n that has been computed */
static size_t cap     = 0;     /* allocated array size */

/* Ensure all four arrays are large enough for indices up to n. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap)
        return;                     /* already sufficient */

    size_t old_cap = cap;
    size_t new_cap = (size_t)n + 1;
    p_arr  = (seq_t *)realloc(p_arr,  new_cap * sizeof(seq_t));
    q_arr  = (seq_t *)realloc(q_arr,  new_cap * sizeof(seq_t));
    sp_arr = (seq_t *)realloc(sp_arr, new_cap * sizeof(seq_t));
    sd_arr = (seq_t *)realloc(sd_arr, new_cap * sizeof(seq_t));
    if (!p_arr || !q_arr || !sp_arr || !sd_arr)
        abort();                    /* allocation failed */

    /* Initialise newly allocated entries to zero (p_arr[0] set later) */
    for (size_t i = old_cap; i < new_cap; ++i) {
        p_arr[i] = 0;
        q_arr[i] = 0;
        sp_arr[i] = 0;
        sd_arr[i] = 0;
    }

    cap = new_cap;
}

/* Compute all required values up to n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= max_done)
        return;                     /* already computed */

    ensure_capacity(n);

    /* Reset arrays for the new range */
    for (idx_t i = 0; i <= n; ++i) {
        p_arr[i] = 0;
        q_arr[i] = 0;
    }
    p_arr[0] = 1;   /* p(0) = 1 */
    q_arr[0] = 1;   /* q(0) = 1 */

    /* Compute unrestricted partitions p(k)  */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            p_arr[(size_t)sum] += p_arr[(size_t)(sum - part)];
        }
    }

    /* Compute partitions into distinct parts q(k) */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            q_arr[(size_t)sum] += q_arr[(size_t)(sum - part)];
        }
    }

    /* Recompute partial sums */
    sd_arr[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sd_arr[(size_t)i] = sd_arr[(size_t)(i - 1)] + q_arr[(size_t)i];
    }

    sp_arr[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sp_arr[(size_t)i] = sp_arr[(size_t)(i - 1)] + p_arr[(size_t)i];
    }

    max_done = n;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is defined for n ≥ 1 */
    compute_up_to(n);
    return sp_arr[(size_t)n] - sd_arr[(size_t)n];
}
