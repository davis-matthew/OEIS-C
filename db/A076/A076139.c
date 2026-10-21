/*
 * A076139  Triangular numbers that are one-third of another triangular
 *          number: T(m) such that 3*T(m) = T(k) for some k.
 * https://oeis.org/A076139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; a(n)=14*a(n-1)-a(n-2)+1 (n≥2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a_prev = (seq_t)0;   /* a(0) */
    if (n == 0) return a_prev;

    seq_t a_curr = (seq_t)1;   /* a(1) */
    if (n == 1) return a_curr;

    /* Iteratively apply the recurrence up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)14 * a_curr - a_prev + (seq_t)1;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
