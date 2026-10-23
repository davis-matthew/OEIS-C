/*
 * A026589  a(n) = T(n,n-4), T given by A026584. Also a(n) = number of integer
 *          strings s(0),...,s(n) counted by T, such that s(n)=4.
 * https://oeis.org/A026589
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A026584(n, n-4).  Recurrence (valid for n ≥ 10):
 *   -(n+4)*(65*n-269)*a(n)
 *   +(-65*n^2+140*n+1933)*a(n-1)
 *   +(809*n^2-2431*n-4514)*a(n-2)
 *   +(-123*n^2+2496*n-205)*a(n-3)
 *   +2*(-726*n^2+3737*n-4395)*a(n-4)
 *   +8*(56*n-215)*(2*n-9)*a(n-5) = 0
 * Initial values (n = 4…9):
 *   a(4)=1, a(5)=2, a(6)=9, a(7)=22, a(8)=69, a(9)=178
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 4) return 0;

    /* exact values for n = 4 … 9 */
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)2;
    if (n == 6) return (seq_t)9;
    if (n == 7) return (seq_t)22;
    if (n == 8) return (seq_t)69;
    if (n == 9) return (seq_t)178;

    /* buffer holding a(k‑5) … a(k‑1) for the current k */
    seq_t buf[5];
    buf[0] = (seq_t)2;    /* a(5) */
    buf[1] = (seq_t)9;    /* a(6) */
    buf[2] = (seq_t)22;   /* a(7) */
    buf[3] = (seq_t)69;   /* a(8) */
    buf[4] = (seq_t)178;  /* a(9) */

    /* iterate from k = 10 up to n */
    for (idx_t k = 10; k <= n; ++k)
    {
        /* polynomial coefficients (int64_t is wide enough) */
        int64_t c1 = -65 * (int64_t)k * (int64_t)k + 140 * (int64_t)k + 1933;
        int64_t c2 =  809 * (int64_t)k * (int64_t)k - 2431 * (int64_t)k - 4514;
        int64_t c3 = -123 * (int64_t)k * (int64_t)k + 2496 * (int64_t)k - 205;
        int64_t c4 =  2 * (-726 * (int64_t)k * (int64_t)k + 3737 * (int64_t)k - 4395);
        int64_t c5 =  8 * (56 * (int64_t)k - 215) * (2 * (int64_t)k - 9);

        seq_t nxt;

        /* use 128‑bit intermediate for small seq_t types */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 pos = 0, neg = 0;

            /* a(k-1) */
            if (c1 >= 0)
                pos += (unsigned __int128)buf[4] * (unsigned __int128)c1;
            else
                neg += (unsigned __int128)buf[4] * (unsigned __int128)(-c1);

            /* a(k-2) */
            if (c2 >= 0)
                pos += (unsigned __int128)buf[3] * (unsigned __int128)c2;
            else
                neg += (unsigned __int128)buf[3] * (unsigned __int128)(-c2);

            /* a(k-3) */
            if (c3 >= 0)
                pos += (unsigned __int128)buf[2] * (unsigned __int128)c3;
            else
                neg += (unsigned __int128)buf[2] * (unsigned __int128)(-c3);

            /* a(k-4) */
            if (c4 >= 0)
                pos += (unsigned __int128)buf[1] * (unsigned __int128)c4;
            else
                neg += (unsigned __int128)buf[1] * (unsigned __int128)(-c4);

            /* a(k-5) */
            if (c5 >= 0)
                pos += (unsigned __int128)buf[0] * (unsigned __int128)c5;
            else
                neg += (unsigned __int128)buf[0] * (unsigned __int128)(-c5);

            unsigned __int128 denom =
                (unsigned __int128)(k + 4) * (unsigned __int128)(65 * (int64_t)k - 269);
            unsigned __int128 numer = pos - neg;

            nxt = (seq_t)(numer / denom);
        }
        else
        {
            /* arbitrary‑precision path (big integer class) */
            seq_t pos = 0, neg = 0;

            if (c1 >= 0)
                pos += (seq_t)buf[4] * (seq_t)c1;
            else
                neg += (seq_t)buf[4] * (seq_t)(-c1);

            if (c2 >= 0)
                pos += (seq_t)buf[3] * (seq_t)c2;
            else
                neg += (seq_t)buf[3] * (seq_t)(-c2);

            if (c3 >= 0)
                pos += (seq_t)buf[2] * (seq_t)c3;
            else
                neg += (seq_t)buf[2] * (seq_t)(-c3);

            if (c4 >= 0)
                pos += (seq_t)buf[1] * (seq_t)c4;
            else
                neg += (seq_t)buf[1] * (seq_t)(-c4);

            if (c5 >= 0)
                pos += (seq_t)buf[0] * (seq_t)c5;
            else
                neg += (seq_t)buf[0] * (seq_t)(-c5);

            seq_t denom = (seq_t)(k + 4) * (seq_t)(65 * (int64_t)k - 269);
            seq_t numer = pos - neg;

            nxt = numer / denom;
        }

        /* shift buffer: drop a(k-5) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = nxt;
    }

    return buf[4];   /* a(n) */
}
