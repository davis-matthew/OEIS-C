/*
 * A141379  a(n) = the smallest positive integer non-coprime to both n and
 *          phi(n), where phi(n) is the number of positive integers that are
 *          <= n and are coprime to n.
 * https://oeis.org/A141379
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest positive integer d such that gcd(d, n) > 1
 *            and gcd(d, φ(n)) > 1
 * Range:     u8 n <= 130, u16 n <= 8966, u32 n <= 8966, u64 n <= 8966,
 *            u128 n <= 8966, big any n
 * Helpers:   gcd, phi64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>
#include "../utils/phi64.h"

seq_t a(idx_t n) {
    /* For values below the offset we return 0; the sequence is defined
       only for n ≥ 3. */
    if (n < 3) return 0;

    seq_t N = (seq_t)n;                 /* cast index to seq_t */
    seq_t phiN = (seq_t)phi64((uint64_t)n);

    seq_t d = 2;                /* start testing from the smallest d > 1 */
    for (;;) {
        if (gcd(d, N) != 1 && gcd(d, phiN) != 1)
            return d;
        ++d;
    }
}
