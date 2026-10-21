/*
 * A206825  Number of solutions (n,k) of k^4=n^4 (mod n), where 1<=k<n.
 * https://oeis.org/A206825
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)/∏_{p|n} p^{ceil(v_p(n)/4)})
 * Range:     u8 n <= 1052, u16 n <= 9569, u32 n <= 9569, u64 n <= 9569,
 *            u128 n <= 9569, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v_p(n) is the exponent of prime p in n. */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t L = 1;
    idx_t m = n;

    /* factor out 2 */
    if ((m & 1) == 0) {
        int e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        int ceil = (e + 3) / 4;
        L *= pow_seq((seq_t)2, ceil);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        int ceil = (e + 3) / 4;
        L *= pow_seq((seq_t)p, ceil);
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        L *= pow_seq((seq_t)m, 1); /* ceil((1+3)/4) = 1 */
    }

    seq_t res = ((seq_t)(n - 1)) / L;
    return res;
}
