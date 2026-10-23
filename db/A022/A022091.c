/*
 * A022091  Fibonacci sequence beginning 0, 8.
 * https://oeis.org/A022091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 8·F(n) = a(n-1)+a(n-2)  with a(0)=0, a(1)=8  */
seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return 0;
    if (n == 1) return 8;

    seq_t a_prev = 0;   /* a(0) */
    seq_t a_curr = 8;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_next = a_prev + a_curr;   /* a(n) = a(n-1)+a(n-2) */
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
