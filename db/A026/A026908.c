/*
 * A026908  a(n) = A026907(2*n, n).
 * https://oeis.org/A026908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) + 3*binomial(2*n+4, n+2) - 18 (OEIS
 *            A026908)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   _safe_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_safe_binom.h"

seq_t a(idx_t n)
{
    /* binomial(2*n, n) */
    seq_t first = _safe_binom(2 * n, n);

    /* binomial(2*n+4, n+2) */
    seq_t second = _safe_binom(2 * n + 4, n + 2);

    /* a(n) = first + 3*second - 18 */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)first
                               + (unsigned __int128)3 * (unsigned __int128)second
                               - (unsigned __int128)18;
        return (seq_t)tmp;
    } else {
        seq_t tmp = first + (seq_t)3 * second - (seq_t)18;
        return tmp;
    }
}
