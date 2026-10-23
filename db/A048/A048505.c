/*
 * A048505  Array T read by antidiagonals, n-th difference of
 *          (T(k,n),T(k,n-1),...,T(k,0)) is (k+n)^2, for n=1,2,3,...;
 *          k=0,1,2,...
 * https://oeis.org/A048505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A048505 – Array T read by antidiagonals.
    T(k, n) = (n^2 + (4k+1)n + (2k)^2) * 2^(n-2) - k^2 + 1.
    a(n) is the nth entry when the two‑dimensional array T is read
    along its antidiagonals (k + n = constant).  The function returns
    the value for any n ≥ 0.  It uses no external utilities and is
    fully portable to C11 and C++17.  */
#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)).  */
static idx_t isqrt_u128(unsigned __int128 x)
{
    if (x <= 1)
        return (idx_t)x;
    unsigned __int128 r = x;
    while (1) {
        unsigned __int128 t = (r + x / r) >> 1;
        if (t >= r)
            return (idx_t)r;
        r = t;
    }
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that
       s*(s+1)/2 <= n < (s+1)*(s+2)/2.  */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t root = isqrt_u128(tmp);
    idx_t s = (root - 1) / 2;

    /* Coordinates on the antidiagonal.  */
    idx_t prefix = s * (s + 1) / 2;
    idx_t k = n - prefix;      /* 0 <= k <= s */
    idx_t m = s - k;           /* the n‑coordinate of T(k, m) */

    /* Compute the numerator  n^2 + (4k+1)n + (2k)^2  in 128‑bit.  */
    unsigned __int128 sk = (unsigned __int128)k;
    unsigned __int128 sn = (unsigned __int128)m;
    unsigned __int128 num = sn * sn
                           + ((unsigned __int128)(4 * sk + 1)) * sn
                           + (unsigned __int128)4 * sk * sk;

    /* Convert to the result type and apply the 2^(m-2) factor. */
    seq_t term = (seq_t)num;          /* truncates if num > seq_t max (not needed for exact cases) */
    if (m >= 2) {
        /* Expand by repeated left shifts to avoid large shift counts. */
        for (idx_t i = 0; i < m - 2; ++i)
            term <<= 1;
    } else {
        idx_t d = 2 - m;              /* m = 0 or 1 */
        seq_t divisor = (seq_t)1;
        for (idx_t i = 0; i < d; ++i)
            divisor <<= 1;
        term /= divisor;
    }

    /* Final term is term - k^2 + 1. */
    term -= (seq_t)(sk * sk);
    term += (seq_t)1;

    return term;
}
