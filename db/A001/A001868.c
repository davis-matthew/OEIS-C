/*
 * A001868  Number of n-bead necklaces with 4 colors.
 * https://oeis.org/A001868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n)*Sum_{d|n} phi(d)*4^(n/d), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   a001868_phi, a001868_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001868_phi.h"
#include "../utils/a001868_term.h"

seq_t a(idx_t n) {
    seq_t sumq = 0;
    unsigned __int128 sumr = 0;
    idx_t d;
    if (n == 0) return 1;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            seq_t q;
            uint64_t r;
            a001868_term(a001868_phi(d), n / d, n, &q, &r);
            sumq += q;
            sumr += r;
        }
    }
    sumq += (seq_t)(uint64_t)(sumr / n);
    return sumq;
}
