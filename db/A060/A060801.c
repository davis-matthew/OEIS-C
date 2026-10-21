/*
 * A060801  Invert transform of odd numbers: a(n) = Sum_{k=1..n}
 *          (2*k+1)*a(n-k), a(0)=1.
 * https://oeis.org/A060801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) – 2*a(n-2) (a(0)=1, a(1)=3, a(2)=14)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 14;

    seq_t a_prev = 3;   /* a(n-1) for current n */
    seq_t a_curr = 14;  /* a(n)   for current n */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a_next = 5 * a_curr - 2 * a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
