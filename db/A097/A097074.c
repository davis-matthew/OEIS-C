/*
 * A097074  Expansion of (1-x+2*x^2)/((1-x)*(1-x-2*x^2)).
 * https://oeis.org/A097074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A097074
 * Recurrence: a(0) = 1, a(1) = 1
 *              a(n) = a(n-1) + 2*a(n-2) + 2  for n ≥ 2              */

/* Computes the n-th term of A097074 */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* iteratively compute up to n using the recurrence */
    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)1; /* a(n-1) */
    seq_t a_n   = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 + (seq_t)2 * a_nm2 + (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
