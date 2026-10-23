/*
 * A134016  Inverse score permutation of an Fibonacci -anti-Fibonacci zero sum
 *          game of 2 X 2 matrices.
 * https://oeis.org/A134016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = 4*a(n-2) + a(n-4) with initial values a(1)=2, a(2)=6,
 *            a(3)=10, a(4)=23
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A134016 */
seq_t a(idx_t n)
{
    /* handle the first four indices directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)23;

    /* iterate up to n using the recurrence, keeping only four
       consecutive values to avoid large memory usage */
    seq_t a1 = (seq_t)2;   /* a(1)   */
    seq_t a2 = (seq_t)6;   /* a(2)   */
    seq_t a3 = (seq_t)10;  /* a(3)   */
    seq_t a4 = (seq_t)23;  /* a(4)   */
    seq_t ai = a4;         /* placeholder for a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)4 * a3 + a1;   /* a(i) = 4*a(i-2) + a(i-4) */
        /* shift window: a(i-3) <- a(i-4), a(i-2) <- a(i-3), a(i-1) <- a(i-2), a(i) <- new value */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return ai;
}
