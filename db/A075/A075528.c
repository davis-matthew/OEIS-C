/*
 * A075528  Triangular numbers that are half other triangular numbers.
 * https://oeis.org/A075528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=3; for n≥2: a(n)=34*a(n-1)-a(n-2)+3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from the generating function 3*x/((1-x)*(1-34*x+x^2)). */
seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)3;

    seq_t a_prev = (seq_t)0;
    seq_t a_curr = (seq_t)3;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (seq_t)34 * a_curr - a_prev + (seq_t)3;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
