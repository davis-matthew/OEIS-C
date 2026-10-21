/*
 * A203945  Symmetric matrix based on (1,0,0,1,0,0,1,0,0,...), by
 *          antidiagonals.
 * https://oeis.org/A203945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = M(i,j) where M(i,j) is the number of k (1 ≤ k ≤ min(i,j))
 *            such that k ≡ i ≡ j (mod 3). The pair (i,j) is obtained by
 *            listing the infinite matrix M in antidiagonal order starting
 *            from (1,1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 return 0. */
    if (n == 0)
        return 0;

    /* Find the diagonal sum S with (S-1)*S/2 ≥ n. */
    idx_t S = 2;
    while ((S - 1) * S / 2 < n)
        ++S;

    /* Total elements up to the previous diagonal (S‑1). */
    idx_t prev_total = (S - 2) * (S - 1) / 2;

    /* Offset within the current diagonal, zero‑based. */
    idx_t offset = n - prev_total - 1;

    /* Convert offset to matrix coordinates (i,j). */
    idx_t i = offset + 1;
    idx_t j = S - i;

    /* Compute M(i,j) according to the definition. */
    if (i % 3 != j % 3)
        return 0;

    idx_t r = i % 3;
    if (r == 0)        /* smallest positive k with k ≡ 0 (mod 3) is 3. */
        r = 3;

    idx_t m = (i < j) ? i : j;   /* min(i,j) */
    if (m < r)
        return 0;

    idx_t cnt = (m - r) / 3 + 1;
    return (seq_t)cnt;
}
