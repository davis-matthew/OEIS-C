/*
 * A056457  Palindromes using exactly six different symbols.
 * https://oeis.org/A056457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 720 * S( ceil(n/2), 6 ), where S(m,k) is the Stirling
 *            number of the second kind
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses the recurrence
    S(m,k) = k * S(m‑1,k) + S(m‑1,k‑1)   (k ≥ 1, S(0,0)=1, S(0,k)=0 for k>0).
   It works for any unsigned integer type seq_t and index type idx_t. */

seq_t a(idx_t n)
{
    /* m = ceil(n/2)  (for n≥1 this is (n+1)/2 with integer division) */
    idx_t m = (n + 1) / 2;

    /* prev[k] holds S(current i-1, k) during the iteration */
    seq_t prev[7] = {0};
    prev[0] = 1;    /* S(0,0)=1, all other S(0,k)=0 */

    /* Compute S(i,6) for i = 1 … m */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t next[7] = {0};

        /* k = 1 … 6 */
        for (int k = 1; k <= 6; ++k) {
            next[k] = (seq_t)k * prev[k] + prev[k - 1];
        }

        /* move next row into prev for the next iteration */
        for (int k = 0; k <= 6; ++k) prev[k] = next[k];
    }

    /* a(n) = 720 * S(m,6) */
    return prev[6] * (seq_t)720;
}
