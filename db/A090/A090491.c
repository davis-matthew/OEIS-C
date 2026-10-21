/*
 * A090491  G.f.: (1+x^3+x^4+x^5+x^6+x^9)/((1-x)*(1-x^2)^2*(1-x^3)*(1-x^4)).
 * https://oeis.org/A090491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11) a(0)=1, a(1)=1, a(2)=3, a(3)=5, a(4)=10,
 *            a(5)=15, a(6)=26, a(7)=37, a(8)=57, a(9)=79, a(10)=112
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial seven values (up to a(10)) */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)10,
        (seq_t)15, (seq_t)26, (seq_t)37, (seq_t)57, (seq_t)79, (seq_t)112
    };

    if (n < 11)
        return init[n];

    /* Buffer holding the last 11 sequence values:
       a0..a10 correspond to a[n-10] .. a[n] for the current step.   */
    seq_t a0  = init[0];
    seq_t a1  = init[1];
    seq_t a2  = init[2];
    seq_t a3  = init[3];
    seq_t a4  = init[4];
    seq_t a5  = init[5];
    seq_t a6  = init[6];
    seq_t a7  = init[7];
    seq_t a8  = init[8];
    seq_t a9  = init[9];
    seq_t a10 = init[10];

    for (idx_t i = 11; i <= n; ++i)
    {
        /* Compute the next term using the recurrence.               */
        seq_t next = ((seq_t)2 * a10)               /* 2*a(n-1)  */
                     - a8                         /* - a(n-3)  */
                     - ((seq_t)2 * a6)            /* - 2*a(n-5)*/
                     + ((seq_t)2 * a5)            /* + 2*a(n-6)*/
                     + a3                         /* + a(n-8)  */
                     - ((seq_t)2 * a1)            /* - 2*a(n-10)*/
                     + a0;                        /* + a(n-11) */

        /* Roll the window: shift all values one position.            */
        a0  = a1;  a1  = a2;  a2  = a3;  a3  = a4;
        a4  = a5;  a5  = a6;  a6  = a7;  a7  = a8;
        a8  = a9;  a9  = a10; a10 = next;
    }

    return a10;
}
