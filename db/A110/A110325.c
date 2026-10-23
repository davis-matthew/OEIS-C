/*
 * A110325  Row sums of number triangle related to the Jacobsthal numbers.
 * https://oeis.org/A110325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 128, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3);
   Initial values: a(0)=1, a(1)=0, a(2)=-5 */
seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return -5;

    /* Compute iteratively using the recurrence.
       This avoids large intermediate multiplications. */
    seq_t a0 = 1;     /* a(n-3)  */
    seq_t a1 = 0;     /* a(n-2)  */
    seq_t a2 = -5;    /* a(n-1)  */
    seq_t an = 0;     /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        an = 3 * a2 - 3 * a1 + a0; /* a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
