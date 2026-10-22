/*
 * A001785  Second-order reciprocal Stirling number (Fekete) a(n) = [[2n+4,
 *          n]]. The number of n-orbit permutations of a (2n+4)-set with at
 *          least 2 elements in each orbit. Also known as associated Stirling
 *          numbers of the first kind (e.g., Comtet).
 * https://oeis.org/A001785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1113 + 1447*n + 600*n^2 +
 *            80*n^3)*(2*n+4)!/(1215*2^(n+3)*(n-1)!) for n > 0, a(0) = 1;
 *            computed as P(n)*b(n)/1215 with b(n) = n*(n+1)*(n+2)/2 *
 *            (2n+3)!!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* b(n) = n(n+1)(n+2)/2 * product of odd k for k = 1..2n+3; every intermediate is <= b(n) < a(n) */
    uint64_t t = (uint64_t)n * (uint64_t)(n + 1) / 2 * (uint64_t)(n + 2);
    seq_t b = (seq_t)t;
    for (idx_t k = 3; k <= 2 * n + 3; k += 2)
        b *= (seq_t)k;

    /* P(n) = 80n^3 + 600n^2 + 1447n + 1113, kept native */
    unsigned __int128 N = n;
    unsigned __int128 P = 80 * N * N * N + 600 * N * N + 1447 * N + 1113;

    /* a = P*b/1215 = P*q + P*r/1215, where b = 1215*q + r */
    seq_t q = b / 1215;
    unsigned __int128 r = (unsigned __int128)(unsigned long long)(b % 1215);
    unsigned __int128 extra = P * r / 1215;

    seq_t Ps = (seq_t)(uint64_t)P;
    seq_t res = Ps * q;
    res += (seq_t)(uint64_t)extra;
    return res;
}
