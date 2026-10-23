/*
 * A169100  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37 * (a(n-1)+…+a(n-24)) – 703 * a(n-25) for n ≥ 25 with
 *            initial values derived from the generating function: a(0) = 1
 *            a(n) = 37 * (a(0)+…+a(n-1)) + 2 for 1 ≤ n ≤ 24 a(25) = 37 *
 *            (a(24)+…+a(1)) – 703 * a(0) + 1 These initial terms generate the
 *            rest of the sequence. The function memoizes res...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal storage for memoized values */
static seq_t *seq = NULL;      /* array of a(0), a(1), … */
static idx_t  alloc_cap = 0;   /* current allocation size */
static idx_t  len = 0;         /* number of terms already computed */

/* Grow the allocation to at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (alloc_cap >= needed) return;
    idx_t new_cap = alloc_cap ? alloc_cap << 1 : 32;
    if (new_cap < needed) new_cap = needed;
    seq = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
    alloc_cap = new_cap;
}

/* Returns the n‑th term of A169100. */
seq_t a(idx_t n)
{
    if (n < len) return seq[n];

    /* Compute terms up to n. */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            /* a(0) = 1 */
            val = 1;
        } else if (i <= 24) {
            /* a(i) = 37 * sum_{k=0}^{i-1} a(k) + 2 */
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k)
                sum += seq[k];
            val = sum * 37 + 2;
        } else if (i == 25) {
            /* a(25) = 37 * sum_{k=1}^{24} a(25-k) – 703 * a(0) + 1 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 24; ++k)
                sum += seq[i - k];
            val = sum * 37 - 703 * seq[0] + 1;
        } else {
            /* a(i) = 37 * sum_{k=1}^{24} a(i-k) – 703 * a(i-25) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 24; ++k)
                sum += seq[i - k];
            val = sum * 37 - 703 * seq[i - 25];
        }

        /* Ensure capacity and store the computed value. */
        if (alloc_cap < i + 1)
            ensure_capacity(i + 1);
        seq[i] = val;
    }
    /* Update length to reflect the newly computed range. */
    len = n + 1;
    return seq[n];
}
