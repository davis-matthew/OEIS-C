/*
 * A203626  Indices of decagonal numbers which are also octagonal.
 * https://oeis.org/A203626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 194*a(n-1) - a(n-2) - 72, a(1)=1, a(2)=117
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A203626. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)117;

    seq_t prev2 = (seq_t)1;      /* a(n-2) */
    seq_t prev1 = (seq_t)117;    /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)194 * prev1 - prev2 - (seq_t)72;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
