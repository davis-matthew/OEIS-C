/*
 * A061174  Fifth column of Lucas bisection triangle (odd part).
 * https://oeis.org/A061174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   G(x) = (3-2x)*(16*x^6-56*x^5+181*x^4-306*x^3+171*x^2+110*x+5)/(1-3*x+x^2)^5.
   For n >= 10 :
        a(n) = 15*a(n-1) - 95*a(n-2) + 330*a(n-3) - 685*a(n-4) +
               873*a(n-5) - 685*a(n-6) + 330*a(n-7) - 95*a(n-8) +
               15*a(n-9) - a(n-10)
   Initial values (n = 0 .. 9):
        a(0)=15,  a(1)=545,  a(2)=7043,  a(3)=57560,  a(4)=365045,
        a(5)=1970905,  a(6)=9520315,  a(7)=42385132,
        a(8)=177293730,  a(9)=705980760. */

seq_t a(idx_t n)
{
    /* Base values for n = 0 .. 9 */
    static const seq_t init[10] = {
        15ULL,      545ULL,      7043ULL,      57560ULL,
        365045ULL,  1970905ULL,  9520315ULL,   42385132ULL,
        177293730ULL,705980760ULL
    };

    if (n < 10)
        return init[n];

    /* Holds the last 10 computed terms: a(k-10) .. a(k-1) */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i)
        buf[i] = init[i];

    for (idx_t k = 10; k <= n; ++k)
    {
        seq_t next =  15ULL * buf[9]   /* a(k-1)  */
                   -  95ULL * buf[8]   /* a(k-2)  */
                   + 330ULL * buf[7]   /* a(k-3)  */
                   - 685ULL * buf[6]   /* a(k-4)  */
                   + 873ULL * buf[5]   /* a(k-5)  */
                   - 685ULL * buf[4]   /* a(k-6)  */
                   + 330ULL * buf[3]   /* a(k-7)  */
                   -  95ULL * buf[2]   /* a(k-8)  */
                   +  15ULL * buf[1]   /* a(k-9)  */
                   -      buf[0];      /* a(k-10) */

        /* Shift buffer and insert the new term */
        for (int i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = next;
    }

    return buf[9];   /* a(n) */
}
