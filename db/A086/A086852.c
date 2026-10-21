/*
 * A086852  Number of permutations of length n with exactly 1 rising or
 *          falling succession.
 * https://oeis.org/A086852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  (3-n)*a(n) + (n+1)*(n-3)*a(n-1)
 *                 -(n^2-4n+5)*a(n-2) -(n-1)*(n-5)*a(n-3)
 *                 +(n-1)*(n-2)*a(n-4) = 0
 *                 a(0)=0, a(1)=0, a(2)=2, a(3)=4
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Keep the last four terms of the recurrence */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)0;   /* a(n-3) */
    seq_t a2 = (seq_t)2;   /* a(n-2) */
    seq_t a3 = (seq_t)4;   /* a(n-1) */

    /* Use 128‑bit arithmetic for small integral seq_t types to avoid overflow */
    const int small_seq = (sizeof(seq_t) <= 8);

    for (idx_t i = 4; i <= n; ++i)
    {
        int64_t i_int = (int64_t)i;

        /* Coefficients with the correct signs */
        int64_t c1 = (i_int + 1) * (i_int - 3);                /* (n+1)(n-3)   */
        int64_t c2 = -(i_int * i_int - 4 * i_int + 5);         /* -(n^2-4n+5)  */
        int64_t c3 = -(i_int - 1) * (i_int - 5);              /* -(n-1)(n-5)  */
        int64_t c4 = (i_int - 1) * (i_int - 2);                /*  (n-1)(n-2)  */

        seq_t ai;
        if (small_seq) /* 128‑bit intermediate */
        {
            __int128 t1 = (__int128)a3 * c1;
            __int128 t2 = (__int128)a2 * c2;
            __int128 t3 = (__int128)a1 * c3;
            __int128 t4 = (__int128)a0 * c4;
            __int128 rhs = t1 + t2 + t3 + t4;   /* numerator */
            ai = (seq_t)(rhs / (i - 3));        /* divide by (n-3) */
        }
        else /* big‑int or 128‑bit seq_t: rely on its own operations */
        {
            seq_t t1 = a3 * (seq_t)c1;
            seq_t t2 = a2 * (seq_t)c2;
            seq_t t3 = a1 * (seq_t)c3;
            seq_t t4 = a0 * (seq_t)c4;
            seq_t rhs = t1 + t2 + t3 + t4;      /* numerator */
            ai = rhs / (i - 3);                 /* divide by (n-3) */
        }

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
