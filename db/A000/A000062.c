/*
 * A000062  A Beatty sequence: a(n) = floor(n/(e-2)).
 * https://oeis.org/A000062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/(e-2)), computed exactly with a convergent p/q of
 *            1/(e-2) = [1; 2,1,1, 4,1,1, 6,1,1, ...] where q > n, so a(n) =
 *            floor(n*p/q)
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 pm = 1, qm = 0;   /* p_{-1}, q_{-1} */
    unsigned __int128 p = 1, q = 1;     /* p_0, q_0 (b_0 = 1) */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned long long k = 1;
    while (q <= N) {
        unsigned __int128 b = (k % 3 == 1) ? (unsigned __int128)(2 * ((k + 2) / 3)) : 1;
        unsigned __int128 np = b * p + pm, nq = b * q + qm;
        pm = p; qm = q;
        p = np; q = nq;
        k++;
    }
    /* p/q = 1 + r/q with r = p - q; floor(n*p/q) = n + floor(n*r/q) */
    unsigned __int128 r = p - q;
    uint64_t extra = (uint64_t)((N * r) / q);
    return (seq_t)n + (seq_t)extra;
}
