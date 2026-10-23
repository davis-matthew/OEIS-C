/*
 * A172270  a(n) = floor(n*(sqrt(11)-sqrt(5))).
 * https://oeis.org/A172270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(11)-sqrt(5)))
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 values 
   returns floor(sqrt(x)) */
static seq_t isqrt128(unsigned __int128 x) {
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64); /* 2^64 > sqrt(max) */
    while (high - low > 1U) {
        unsigned __int128 mid = (low + high) >> 1U;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return (seq_t)low;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    unsigned __int128 nn = (unsigned __int128)n * n;
    unsigned __int128 val11 = nn * 11;
    unsigned __int128 val5  = nn * 5;

    seq_t A = isqrt128(val11);   /* floor(n*sqrt(11))   */
    seq_t B = isqrt128(val5);    /* floor(n*sqrt(5))    */

    /* u = A - B is non‑negative for n > 0 */
    __int128 u = (__int128)A - (__int128)B;

    /* Q = 16*n^2 - u^2   (can be negative) */
    __int128 Q = (__int128)16 * nn - u * u;

    if (Q < 0)            /* 2*n^2*sqrt(55) > 16*n^2 - u^2  →  n*α < u  */
        return (seq_t)(u - 1);

    /* Compare 220*n^4 with Q^2  (both positive) */
    unsigned __int128 n4 = (unsigned __int128)n * n * n * n;
    unsigned __int128 left  = (unsigned __int128)220 * n4;   /* 220*n^4 */
    unsigned __int128 Q2    = (unsigned __int128)Q * (unsigned __int128)Q;

    if (left > Q2)        /* n*α < u  →  floor = u-1  */
        return (seq_t)(u - 1);
    else                  /* otherwise floor = u      */
        return (seq_t)u;
}
