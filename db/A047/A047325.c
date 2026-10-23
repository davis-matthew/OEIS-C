/*
 * A047325  Numbers that are congruent to {1, 2, 5, 6} mod 7.
 * https://oeis.org/A047325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + [1,2,5,6][(n-1)%4]
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number that is congruent to 1,2,5 or 6 modulo 7. */
seq_t a(idx_t n)
{
    /* Handle very small n directly.  The formula works for all n>=1. */
    const seq_t offs[4] = {1, 2, 5, 6};

    /* Zero‑based offset from the start of the sequence. */
    idx_t m = n - 1;                 /* n>=1, so m is non‑negative */
    idx_t block = m / 4;             /* how many complete 4‑term blocks */
    idx_t pos   = m % 4;             /* position inside the current block */

    /* Build the result: 7*block + offset.  The multiplication is done in
     * seq_t to avoid overflow beyond the 64‑bit limit of idx_t. */
    seq_t base = (seq_t)block * 7;
    return base + offs[pos];
}
