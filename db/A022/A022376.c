/*
 * A022376  Fibonacci sequence beginning 2, 28.
 * https://oeis.org/A022376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) with a(0)=2, a(1)=28
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return 2;
    if (n == 1) return 28;

    seq_t a0 = 2;   /* a(0) */
    seq_t a1 = 28;  /* a(1) */
    seq_t an;       /* a(n) to be computed */

    /* Iterate using the Fibonacci-like recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        an = a0 + a1; /* a(i) = a(i-2) + a(i-1) */
        a0 = a1;
        a1 = an;
    }
    return an;
}
