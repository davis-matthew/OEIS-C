/*
 * A147852  Expansion of -x*(x+1)*(x^3+x^2+x-1) /(2*x^6+4*x^4-x^2-2*x+1).
 * https://oeis.org/A147852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 36, i64 n <= 72, i128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of -x*(x+1)*(x^3+x^2+x-1) /(2*x^6+4*x^4-x^2-2*x+1)
   The generating function gives the linear recurrence

          a(n) = 2*a(n-1) + a(n-2) - 4*a(n-4) - 2*a(n-6)   for n ≥ 6

   with initial terms a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=10.
   The function below computes a(n) using this recurrence; it uses
   only integer arithmetic allowed by the requirements and works
   for any idx_t/seq_t combination. */

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 5 */
    const seq_t init[6] = { (seq_t)0, (seq_t)1, (seq_t)2,
                            (seq_t)3, (seq_t)6, (seq_t)10 };

    if (n < 6)                 /* direct lookup for small indices   */
        return init[n];

    /* five last values are kept in a rotating buffer:
       a0 = a(k-6), a1 = a(k-5), … , a5 = a(k-1)   */
    seq_t a0 = init[0];   /* a(n-6) */
    seq_t a1 = init[1];   /* a(n-5) */
    seq_t a2 = init[2];   /* a(n-4) */
    seq_t a3 = init[3];   /* a(n-3) */
    seq_t a4 = init[4];   /* a(n-2) */
    seq_t a5 = init[5];   /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) + a(i-2) - 4*a(i-4) - 2*a(i-6)  */
        seq_t an = 2 * a5 + a4 - 4 * a2 - 2 * a0;

        /* rotate the buffer for the next iteration          */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return a5;
}
