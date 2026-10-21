/*
 * A218326  Odd octagonal pyramidal numbers.
 * https://oeis.org/A218326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-(-1)^n-1)*(4*n-(-1)^n-3)*(4*n-(-1)^n-4)/8
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ensure n >= 1 in use; behaviour for n=0 is defined as 0 */
    if (n == 0) return (seq_t)0;

    seq_t base = (seq_t)n * (seq_t)4;       /* 4*n  */
    seq_t t1, t2, t3;

    if (n & 1) /* odd n : (-1)^n = -1  */
    {
        t1 = base;            /* 4*n + 1 - 1 = 4*n            */
        t2 = base - (seq_t)2; /* 4*n + 1 - 3 = 4*n - 2        */
        t3 = base - (seq_t)3; /* 4*n + 1 - 4 = 4*n - 3        */
    }
    else /* even n : (-1)^n = 1  */
    {
        t1 = base - (seq_t)2; /* 4*n - 1 - 1 = 4*n - 2        */
        t2 = base - (seq_t)4; /* 4*n - 1 - 3 = 4*n - 4        */
        t3 = base - (seq_t)5; /* 4*n - 1 - 4 = 4*n - 5        */
    }

    seq_t prod = t1 * t2 * t3;
    return prod / (seq_t)8;
}
