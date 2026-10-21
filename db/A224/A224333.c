/*
 * A224333  T(n,k)=Number of idempotent n X n 0..k matrices of rank n-1.
 * https://oeis.org/A224333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Sequence a(n) = T(n',k') with * T(n,k) = 2*n*(1+k)^(n-1) - n *
 *            the terms are taken from the infinite matrix T(n,k) (n,k ≥ 1) *
 *            in anti‑diagonal order: first all entries with n+k = 2, * then
 *            with n+k = 3, then with n+k = 4, …. * For a given index n, let S
 *            be the smallest integer such that * (S-1)*...
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 130, u64 n <= 369,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find S such that (S-1)*S/2 >= n */
    idx_t S = 2;
    while ((S - 1) * S / 2 < n)
        ++S;

    /* Count of entries up to the previous block */
    idx_t prev = (S - 2) * (S - 1) / 2;
    idx_t offset = n - prev;          /* 1‑based offset within block */
    idx_t n_val = offset;             /* row index in the matrix  */
    idx_t k_val = S - offset;         /* column index in the matrix */

    /* Convert to seq_t */
    seq_t ns = (seq_t)n_val;
    seq_t base = (seq_t)(k_val + 1);  /* (1 + k)   */
    seq_t pow_val = 1;

    /* Compute (1+k)^(n-1) – exponent is small (≤ ~140 for n up to 10 000) */
    for (idx_t i = 0; i < n_val - 1; ++i)
        pow_val *= base;

    /* Evaluate T(n,k) = 2*n*(1+k)^(n-1) - n */
    seq_t res = 2 * ns * pow_val - ns;
    return res;
}
