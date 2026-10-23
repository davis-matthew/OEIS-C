/*
 * A046191  Indices of hexagonal numbers which are also octagonal.
 * https://oeis.org/A046191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) - a(n-2) - 24, a(1) = 1, a(2) = 77
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)77;

    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)77;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = ((seq_t)98 * a_prev1) - a_prev2 - (seq_t)24;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
