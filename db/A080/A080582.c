/*
 * A080582  Ratios of successive terms of A079278.
 * https://oeis.org/A080582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A079278(n+1)/A079278(n) = p(n)+q(n), where b(n)=p(n)/q(n)
 *            in lowest terms, b(1)=1/1, b(n+1)=p*(p+2q)/(q*(p+q))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t p = 1, q = 1;
    idx_t i;
    for (i = 1; i < n; i++) {
        seq_t np = p * (p + q + q);
        seq_t nq = q * (p + q);
        p = np;
        q = nq;
    }
    return p + q;
}
