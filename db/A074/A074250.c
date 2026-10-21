/*
 * A074250  Smallest p>1 for which n^p ends in n, or -1 if no such p exists.
 *          The smallest p for which n is a p-morphic number.
 * https://oeis.org/A074250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A074250
    a(n) = smallest integer p > 1 such that n^p ends in the decimal digits of n;
    if no such p exists the value is -1.
    The algorithm iteratively multiplies n modulo 10^d, where d is the number of
    decimal digits of n.  As soon as the current residue equals n again,
    that exponent p is the required smallest one.  If all possible residues
    (at most 10^d of them) are exhausted before the residue repeats,
    the number has no p‑morphic property, hence -1 is returned.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 */
    uint64_t nn = (uint64_t)n;

    /* find number of decimal digits d */
    unsigned d = 0;
    uint64_t tmp = nn;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }

    /* compute m = 10^d */
    uint64_t m = 1;
    for (unsigned i = 0; i < d; ++i) {
        m *= 10;
    }

    /* iterate powers of n modulo m */
    uint64_t val = nn % m;          /* value of n^1 mod m */
    for (uint64_t p = 2; p <= m; ++p) {
        val = (val * nn) % m;       /* n^p mod m */
        if (val == nn) {
            return (seq_t)p;        /* smallest p > 1 found */
        }
    }
    return (seq_t)-1;               /* no such p exists */
}
