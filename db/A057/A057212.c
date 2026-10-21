/*
 * A057212  n-th run has length n.
 * https://oeis.org/A057212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (1 + (-1)^round(sqrt(2*n))) / 2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = 1 if round(sqrt(2*n)) is even, else 0. */
/* We compute round(sqrt(2*n)) via integer arithmetic. */

#include <stdint.h>
#include <limits.h>

/* Integer square root of a 128‑bit number, returning floor(sqrt(x)). */
static unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned long long lo = 0;
    unsigned long long hi = (1ULL << 33);          /* sqrt(2*(2^64-1)) < 2^32.5 */
    while (lo < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 for valid calls. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 twoN = N << 1;                 /* 2*n */
    unsigned long long t = isqrt_u128(twoN);         /* floor(sqrt(2*n)) */

    /* Decide whether to round up: compare (2*t+1)^2 with 8*n */
    unsigned long long l = (t << 1) + 1;             /* 2*t + 1  */
    unsigned __int128 l2 = (unsigned __int128)l * (unsigned __int128)l;
    unsigned __int128 eightN = N << 3;               /* 8*n */

    unsigned long long r = (l2 > eightN) ? t : (t + 1);

    /* a(n) = 1 if r is even, else 0. */
    return (seq_t)((r & 1ULL) == 0);
}
