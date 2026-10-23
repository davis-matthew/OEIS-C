/*
 * A039301  Number of distinct quadratic residues mod 4^n.
 * https://oeis.org/A039301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4^n + 10)/6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 4*a(n-2)  for n > 2,
   with a(0)=1, a(1)=2, a(2)=4. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */
    seq_t cur;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        cur = (5 * prev1) - (4 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
