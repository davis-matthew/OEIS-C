/*
 * A119957  a(n) is the sum of p consecutive residues of 2^x modulo n,
 *          starting with a sufficiently large x and where p = period of
 *          binary representation of 1/n.
 * https://oeis.org/A119957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{j=x}^{x+P-1} 2^j mod n where P is the period of the
 *            binary expansion of 1/n, and x is the first exponent for which
 *            that period starts. For a factorisation n = 2^k·m with m odd,
 *            the period equals the multiplicative order of 2 modulo m, and
 *            the summation starts at exponent j = k. The se...
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* Separate power of two factor */
    idx_t temp = n;
    idx_t k = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++k;
    }

    /* n is a power of two */
    if (temp == 1)
        return 0;

    idx_t m = temp;          /* odd part of n */

    /* Compute the period P = multiplicative order of 2 modulo m. */
    idx_t P = 1;
    idx_t cur = 2 % m;
    while (cur != 1) {
        cur = (cur * 2) % m;
        ++P;
    }

    /* Starting residue: 2^k mod n */
    seq_t start = 1;
    for (idx_t i = 0; i < k; ++i) {
        start = (start * 2) % (seq_t)n;
    }

    seq_t sum = 0;
    seq_t r   = start;
    for (idx_t i = 0; i < P; ++i) {
        sum += r;
        r = (r * 2) % (seq_t)n;
    }

    return sum;
}
