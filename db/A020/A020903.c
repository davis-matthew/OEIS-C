/*
 * A020903  Lim f(f(...f(n))) where f is the fractal sequence given by
 *          f(n)=A002260(n+1).
 * https://oeis.org/A020903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = limit of iterating f until 1 or 2, where f(n) is the nth
 *            term of the fractal ladder sequence A002260
 *            (1,2,1,2,3,1,2,3,4,1,…)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t f_idx(idx_t n)
{
    if (n <= 2)
        return n;
    idx_t m = n + 1;          /* use n+1 for the triangular test */
    idx_t lo = 1, hi = m;     /* hi is certainly large enough for the needed k */

    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= m)
            hi = mid;
        else
            lo = mid + 1;
    }

    idx_t k = lo;                                   /* k satisfies T_k >= m */
    unsigned __int128 pre128 = (unsigned __int128)k * (k - 1) / 2;
    idx_t pre = (idx_t)pre128;

    return m - pre;                                  /* position in the k‑th ladder */
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)n;

    idx_t cur = n;
    while (cur > 2)                                  /* iterate until reaching 1 or 2 */
        cur = f_idx(cur);
    return (seq_t)cur;
}
