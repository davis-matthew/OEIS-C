/*
 * A159738  Number of permutations of 7 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*n*8^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Recurrence: a(n) = 16*a(n-1) - 64*a(n-2); a(2)=14, a(3)=168) */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)168;

    seq_t prev2 = (seq_t)14;   /* a(2) */
    seq_t prev1 = (seq_t)168;  /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)16 * prev1 - (seq_t)64 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
