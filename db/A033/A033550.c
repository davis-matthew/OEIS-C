/*
 * A033550  a(n) = A005248(n) - n.
 * https://oeis.org/A033550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + n - 1, a(0)=2, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initialise the first two terms */
    seq_t a0 = (seq_t)2;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */

    /* Trivial cases */
    if (n == 0) return a0;
    if (n == 1) return a1;

    /* Iterate the recurrence up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = a1 * (seq_t)3;   /* 3*a(n-1) */
        temp -= a0;                  /* - a(n-2)   */
        temp += (seq_t)(i - 1);      /* + n - 1    */
        a0 = a1;                     /* shift window */
        a1 = temp;
    }

    return a1;    /* which now holds a(n) */
}
