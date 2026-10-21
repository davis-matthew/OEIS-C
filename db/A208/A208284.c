/*
 * A208284  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n^5 + 54*n^4 + 91*n^3 - 54*n^2) / 6 (OEIS A208284)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 85, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute intermediate using 128‑bit integer to avoid overflow */
    __int128 N = n;                 /* cast index to 128‑bit */
    __int128 num = 5;
    num *= N;   /* 5*n        */
    num *= N;   /* 5*n^2      */
    num *= N;   /* 5*n^3      */
    num *= N;   /* 5*n^4      */
    num *= N;   /* 5*n^5      */

    __int128 t = N * N * N * N;     /* n^4  */
    t *= 54;                        /* 54*n^4 */
    num += t;

    t = N * N * N;                  /* n^3  */
    t *= 91;                        /* 91*n^3 */
    num += t;

    t = N * N;                      /* n^2  */
    t *= 54;                        /* 54*n^2 */
    num -= t;

    /* exact division by 6 */
    num /= 6;

    return (seq_t)num;
}
