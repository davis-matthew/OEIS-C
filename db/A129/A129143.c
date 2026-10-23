/*
 * A129143  Start with the empty sequence and append in step k the consecutive
 *          numbers 2^(k-1) to 2^(k-1)+k-1.
 * https://oeis.org/A129143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(k-1) + n - k*(k-1)/2 - 1, where k = smallest integer
 *            with k*(k+1)/2 ≥ n (i.e. k = ⌈(√(8n+1)-1)/2⌉). The sequence is
 *            formed by appending, in step k, the k numbers 2^(k-1),
 *            2^(k-1)+1, …, 2^(k-1)+k-1. a(1)=1
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 5151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n=1 */

    /* Find the step k containing position n.  */
    idx_t k = 1;
    /* triangular number T(k) = k*(k+1)/2; use __int128 for safety. */
    __int128 tri = 1;              /* T(1) = 1 */
    while (tri < n) {
        ++k;
        tri = (__int128)k * (k + 1) / 2;
    }

    /* T(k-1) = k*(k-1)/2  (compute once again with __int128). */
    idx_t t_prev = (idx_t)((__int128)k * (k - 1) / 2);

    /* offset of a(n) inside the block of step k (0‑based). */
    idx_t offset = n - t_prev - 1;

    /* base = 2^(k-1)  – compute by repeated multiplication to avoid
       unsigned shift UB when k-1 ≥ width of seq_t. */
    seq_t base = 1;
    for (idx_t i = 0; i < k - 1; ++i) {
        base *= 2;
    }

    return base + (seq_t)offset;
}
