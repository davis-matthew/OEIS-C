/*
 * A210944  Triangle read by rows with T(n,k) = n - A195818(k), n>=1, k>=1, if
 *          (n - A195818(k))>=0.
 * https://oeis.org/A210944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = r - A195818(k), reading rows r = 1,2,3,... of the
 *            triangle T(r,k) = r - A195818(k) for all k >= 1 with A195818(k)
 *            <= r, where A195818(k) = (3*k*(k+1) + (2*k+1)*(-1)^k - 1)/2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A195818(k), generalized 14-gonal numbers m*(6m-5), ascending: 0,1,11,14,34,... */
static idx_t A195818_g(idx_t k)
{
    idx_t t = 3 * k * (k + 1);
    if ((k & 1ULL) == 0)
        t += 2 * k + 1;
    else
        t -= 2 * k + 1;
    t -= 1;
    return t / 2;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;
    idx_t r = 0;
    idx_t len = 0;
    for (;;) {
        ++r;
        while (A195818_g(len + 1) <= r)
            ++len;
        cum += len;
        if (cum >= n)
            break;
    }
    idx_t pos = n - (cum - len) - 1; /* 0-based position in row */
    return (seq_t)(r - A195818_g(pos + 1));
}
