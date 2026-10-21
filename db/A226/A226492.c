/*
 * A226492  a(n) = n*(11*n-5)/2.
 * https://oeis.org/A226492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(11*n-5)/2
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;

    /* n*(11*n-5)/2 can be factored as  
       - if n is even: (n/2)*(11*n-5)  
       - if n is odd:  n*((11*n-5)/2)  */
    if ((sn & 1) == 0) {            /* n even */
        seq_t half = sn >> 1;      /* n/2 */
        return half * (sn * 11 - 5);
    } else {                        /* n odd */
        seq_t temp = (sn * 11 - 5) >> 1; /* (11*n-5)/2 */
        return sn * temp;
    }
}
