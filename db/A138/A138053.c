/*
 * A138053  Expansion of -x^2*(19440*x^4+2160*x^3-2304*x^2-150*x+55)
 *          /((3*x+1)*(6*x-1)*(6*x+1)*(15*x-1)*(12*x^2-1)).
 * https://oeis.org/A138053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is defined by the rational generating function
 *            -x^2*(19440*x^4+2160*x^3-2304*x^2-150*x+55) /
 *            ((3*x+1)*(6*x-1)*(6*x+1)*(15*x-1)*(12*x^2-1). The sequence
 *            satisfies the linear recurrence a(n) = 12*a(n‑1) + 93*a(n‑2) -
 *            576*a(n‑3) - 2592*a(n‑4) + 5184*a(n‑5) + 19440*a(n‑6) (n ≥ 7)
 *            with initial va...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return a(n) for n ≥ 1.  a(0) is not required for the OEIS offset. */
seq_t a(idx_t n)
{
    /* Initial six values of the sequence */
    static const seq_t init[6] = {
        (seq_t)0,           /* a(1) */
        (seq_t)55,          /* a(2) */
        (seq_t)510,         /* a(3) */
        (seq_t)8931,        /* a(4) */
        (seq_t)125082,      /* a(5) */
        (seq_t)1914687      /* a(6) */
    };

    if (n <= 6)                /* Small indices have direct answers */
        return init[n-1];

    /* Roll‑through variables holding the six most recent terms:
       a1 = a(n-6), a2 = a(n-5), … , a6 = a(n-1)  */
    seq_t a1 = init[0], a2 = init[1], a3 = init[2];
    seq_t a4 = init[3], a5 = init[4], a6 = init[5];
    seq_t next = 0;

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute the next term using the recurrence.  The addition of
           positive contributions is performed first to avoid underflow
           when the subsequent subtraction is done. */
        next = (seq_t)12 * a6 + (seq_t)93 * a5
             + (seq_t)5184 * a2 + (seq_t)19440 * a1
             - ((seq_t)576 * a4 + (seq_t)2592 * a3);

        /* Shift the window of six terms forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return next;
}
