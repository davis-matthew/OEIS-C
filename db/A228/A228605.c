/*
 * A228605  The Merrifield-Simmons index of the meta-polyphenylene chain of
 *          length n.
 * https://oeis.org/A228605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 18, a(n) = 16*a(n-1) + 11*a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the generating function: (1+2*x)/(1-16*x-11*x^2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)18;  /* a(1) */
    seq_t curr = 0;
    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)16 * prev1 + (seq_t)11 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
