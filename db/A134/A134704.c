/*
 * A134704  Expansion of g.f.
 *          -x*(10*x^4+12*x^3-x^2-3*x-3)/((x^2+x-1)*(4*x^3+x^2-x-1)).
 * https://oeis.org/A134704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   G(x) = -x * (10x^4 + 12x^3 - x^2 - 3x - 3) /
          ( (x^2 + x - 1) * (4x^3 + x^2 - x - 1) )

   Denominator expands to 4x^5 + 5x^4 - 4x^3 - 3x^2 + 1.
   Cancelling the constant 1 in the denominator gives the linear recurrence
   (valid for n >= 6):

          a(n) = 3*a(n-2) + 4*a(n-3) - 5*a(n-4) - 4*a(n-5)

   Initial values (offset 1):
          a(1)=3, a(2)=3, a(3)=10, a(4)=9, a(5)=17
*/

seq_t a(idx_t n)
{
    /* First five terms – directly listed. */
    static const seq_t init[5] = {3, 3, 10, 9, 17};

    if (n <= 5)            /* offset 1 */
        return init[n - 1];

    /* Iterate forward from the 5th term.
       We keep the last five values in a sliding window
       t0 = a(n-5), t1 = a(n-4), t2 = a(n-3),
       t3 = a(n-2), t4 = a(n-1). */
    seq_t t0 = init[0];    /* a(1)   */
    seq_t t1 = init[1];    /* a(2)   */
    seq_t t2 = init[2];    /* a(3)   */
    seq_t t3 = init[3];    /* a(4)   */
    seq_t t4 = init[4];    /* a(5)   */

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Compute a(i) = 3*t3 + 4*t2 - 5*t1 - 4*t0
           The subtraction never underflows because
           3*t3 + 4*t2 >= 5*t1 + 4*t0 for all terms of this sequence. */
        seq_t next = 3 * t3 + 4 * t2;
        next -= 5 * t1;
        next -= 4 * t0;

        /* Shift the window for the next iteration. */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }

    /* t4 now holds a(n) */
    return t4;
}
