/*
 * A156093  One ninth of the alternating sum of the squares of the first n
 *          Fibonacci numbers with index divisible by 4.
 * https://oeis.org/A156093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * F(4n) * F(4n+4) / 63 Recurrence: a(n) +
 *            47*a(n-1) + a(n-2) = (-1)^n Base values: a(0) = 0, a(1) = -1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;

    /* a(n-2), a(n-1) for the recurrence; start with base cases */
    seq_t a0 = (seq_t)0;          /* a(0) */
    seq_t a1 = (seq_t)-1;         /* a(1) */
    seq_t an;                    /* a(n) being computed */

    /* Iterate from 2 up to n, applying the recurrence
       a(n) = (-1)^n - 47*a(n-1) - a(n-2)   */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t sign = ((i & 1ULL) == 0) ? (seq_t)1 : (seq_t)-1;
        an = sign - (seq_t)47 * a1 - a0;  /* compute a(i) */
        a0 = a1;                          /* shift a(n-2) <- a(n-1) */
        a1 = an;                          /* shift a(n-1) <- a(n)   */
    }

    /* a1 now holds a(n) */
    return a1;
}
