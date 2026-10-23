/*
 * A192232  Constant term of the reduction of n-th Fibonacci polynomial by x^2
 *          -> x+1. (See Comments.)
 * https://oeis.org/A192232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = constant term of the reduction of the n-th Fibonacci
 *            polynomial by the replacement x^2 → x + 1. Recurrence with
 *            auxiliary variables: a(1) = 1, a(2) = 0, a(3) = 2, a(4) = 1 for
 *            n ≥ 5: a(n) = a(n-1) + 3·a(n-2) – a(n-3) – a(n-4)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 62, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return 0;           /* not part of the sequence, safety */
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 2;
    if (n == 4) return 1;

    /* store a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 0;   /* a(2) */
    seq_t a3 = 2;   /* a(3) */
    seq_t a4 = 1;   /* a(4) */

    for (idx_t k = 5; k <= n; ++k) {
        /* a(k) = a4 + 3*a3 - a2 - a1 */
        seq_t an = a4 + 3 * a3;
        an -= a2;
        an -= a1;

        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;   /* a(n) */
}
