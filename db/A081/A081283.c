/*
 * A081283  An interleaved sequence of pyramidal and polygonal numbers.
 * https://oeis.org/A081283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (2*n+1-(-1)^n)*(2*n+5-(-1)^n)*(2*n^2+2*(5+(-1)^n)*n+27-11*(-1)^n)/1536
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 150, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t t1, t2, t3;

    /* (-1)^n is 1 if n even, -1 if n odd */
    if ((n & 1) == 0) /* even n */
    {
        t1 = 2 * nn;                /* 2*n          */
        t2 = 2 * nn + 4;            /* 2*n + 4      */
        t3 = 2 * nn * nn + 12 * nn + 16;  /* 2*n^2 + 12*n + 16 */
    }
    else /* odd n */
    {
        t1 = 2 * nn + 2;            /* 2*n + 2      */
        t2 = 2 * nn + 6;            /* 2*n + 6      */
        t3 = 2 * nn * nn + 8 * nn + 38;   /* 2*n^2 + 8*n + 38  */
    }

    /* compute product and divide by 1536 (= 2^9 * 3) */
    return (t1 * t2 * t3) / 1536;
}
