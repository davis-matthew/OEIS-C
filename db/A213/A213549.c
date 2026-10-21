/*
 * A213549  Principal diagonal of the convolution array A213548.
 * https://oeis.org/A213549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(11*n^2 + 3*n - 2)/24
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 310, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t1 = ns * (ns + 1);            /* n*(n+1) */
    seq_t n2 = ns * ns;                  /* n^2      */
    seq_t t2 = (seq_t)11 * n2 + (seq_t)3 * ns - (seq_t)2; /* 11*n^2 + 3*n - 2 */
    seq_t den = 24;                       /* 2^3 * 3  */

    /* Remove the three 2‑factors from t1 or t2 */
    for (int i = 0; i < 3 && den >= 2; ++i)
    {
        if ((t1 & 1) == 0)
        {
            t1 >>= 1;
            den >>= 1;
        }
        else if ((t2 & 1) == 0)
        {
            t2 >>= 1;
            den >>= 1;
        }
        else
            break;  /* in practice this never happens */
    }

    /* Remove the single 3‑factor */
    if (den >= 3)
    {
        if (t1 % 3 == 0)
        {
            t1 /= 3;
            den /= 3;
        }
        else if (t2 % 3 == 0)
        {
            t2 /= 3;
            den /= 3;
        }
    }

    return (t1 * t2) / den;
}
