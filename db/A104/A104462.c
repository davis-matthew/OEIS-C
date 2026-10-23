/*
 * A104462  Convert the binary strings in A101305 to decimal.
 * https://oeis.org/A104462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{n-1} 2^((n(n+3) - (i+1)(i+2)) / 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binary string of length L = n(n+3)/2 bits has 1‑bits at
   positions L-1, L-1-2, L-1-2-3, …, descending by 2,3,4,…,n. */
seq_t a(idx_t n) {
    seq_t res = 0;
    if (n == 0) return 0;

    /* L = n(n+3)/2 */
    seq_t L = ((seq_t)n * (n + 3)) / 2;

    for (idx_t i = 0; i < n; ++i) {
        /* subtract = (i+1)(i+2)/2 */
        seq_t subtract = ((seq_t)(i + 1) * (i + 2)) / 2;
        /* position of the 1‑bit */
        idx_t pos = (idx_t)(L - subtract);
        res += ((seq_t)1) << pos;
    }
    return res;
}
