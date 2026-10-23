/*
 * A138057  Triangle read by rows: row n consists of 0 followed by n (n+1)'s
 *          with alternating signs.
 * https://oeis.org/A138057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 0 if k==0; otherwise (r+1)*(-1)^k, where r = ⌊(√(8*n+1) -
 *            1)/2⌋, k = n - r*(r+1)/2. T(r,0)=0 and for k>0: T(r,k) =
 *            (r+1)*(-1)^k
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        uint64_t cand = r | bit;
        unsigned __int128 prod = (unsigned __int128)cand * cand;
        if (prod <= x)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* 8*n + 1 fits in 128 bits because n < 2^64.
       s = floor(sqrt(8*n + 1))            */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt128(val);

    /* r = (s - 1)/2  (floor division) */
    uint64_t r = (s - 1) / 2;

    /* pre_len = r*(r+1)/2  (fits in 64 bits because r*(r+1)/2 <= n < 2^64) */
    unsigned __int128 pre_len128 = (unsigned __int128)r * (r + 1) / 2;
    uint64_t pre_len = (uint64_t)pre_len128;   /* safe cast */

    uint64_t k = n - pre_len;                  /* 0 <= k <= r */

    if (k == 0)
        return (seq_t)0;

    seq_t value = (seq_t)(r + 1);
    if (k & 1)                 /* odd k => negative sign */
        value = -value;

    return value;
}
