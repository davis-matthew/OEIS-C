/*
 * A073133  Table by antidiagonals of T(n,k) = n*T(n,k-1) + T(n,k-2) starting
 *          with T(n,1) = 1.
 * https://oeis.org/A073133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 73, u32 n <= 182, u64 n <= 453,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073133:  Antidiagonal traversal of the array
 *   T(n,1)=1,   T(n,0)=0
 *   T(n,k)=n*T(n,k-1)+T(n,k-2)   (n,k≥1)
 *
 * The sequence a(n) (n≥1) is defined as the nth entry obtained by
 * reading the table T in antidiagonals with increasing sum i+j, and
 * within each diagonal the columns (k) ascending:
 *
 *   a(n) = T(n_idx, k)
 *   where the diagonal index s is the smallest integer with
 *   (s-1)*s/2 ≥ n,
 *   prev  = (s-1)*(s-2)/2   // entries before diagonal s
 *   offset = n - prev - 1   // zero‑based offset within diagonal
 *   k      = offset + 1
 *   n_idx  = s - k
 *
 * This implementation computes a(n) directly; it performs
 * at most O(√n) multiplications per call. */

#include <stdint.h>

/* Computes T(n,k) for given 1‑based n and k ≥ 1. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t t0 = 0;                 /* T(n,0) */
    seq_t t1 = 1;                 /* T(n,1) */
    if (k == 0) return t0;
    if (k == 1) return t1;
    for (idx_t i = 2; i <= k; ++i) {
        seq_t tn = (seq_t)n * t1 + t0;  /* n*T(n,k-1)+T(n,k-2) */
        t0 = t1;
        t1 = tn;
    }
    return t1;
}

seq_t a(idx_t n)
{
    /* Find smallest s with (s-1)*s/2 ≥ n */
    idx_t s   = 2;
    idx_t cnt = 1;                   /* total entries up to current s */
    while (cnt < n) {
        ++s;
        cnt += s - 1;                /* new diagonal contains s-1 entries */
    }

    /* Determine k and n_idx for the nth element */
    idx_t prev   = cnt - (s - 1);          /* entries before diagonal s */
    idx_t offset = n - prev - 1;           /* zero‑based position in diagonal */
    idx_t k      = offset + 1;             /* column index (1‑based) */
    idx_t n_idx  = s - k;                  /* row index (1‑based) */

    return compute_T(n_idx, k);
}
