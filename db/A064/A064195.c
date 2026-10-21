/*
 * A064195  a(n) = 3*(n-2)*(n-3)*(3*n^2-9)*(3*n^2-9*n-5)/2.
 * https://oeis.org/A064195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-2)*(n-3)*(3*n^2-9)*(3*n^2-9*n-5)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset return 0 (not used in tests) */
    if (n < 2) 
        return (seq_t)0;

    /* Use a 128–bit intermediate to avoid overflow during the
       multiplies.  All operations are performed on the 128‑bit
       type __int128; the final result is cast back to seq_t. */
    __int128 tn   = (__int128)n;
    __int128 t1   = 3 * (tn - 2);                  /* 3*(n-2)          */
    __int128 t2   = tn - 3;                        /* (n-3)             */
    __int128 t3   = 3 * tn * tn - 9;               /* 3*n^2-9           */
    __int128 t4   = 3 * tn * tn - 9 * tn - 5;      /* 3*n^2-9*n-5       */

    __int128 prod = t1 * t2 * t3 * t4;
    prod /= 2;                                   /* divide by 2 */

    return (seq_t)prod;
}
