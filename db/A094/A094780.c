/*
 * A094780  Let 2^k = smallest power of 2 >= binomial(2n,n); a(n) = 2^k -
 *          binomial(2n,n).
 * https://oeis.org/A094780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k - C(2n,n) where k = ceil(log₂(C(2n,n)))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   bitlen_seq_t, central_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A094780. */
#include "../utils/central_binomial.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2n,n). */
    seq_t c = central_binomial(n);

    /* Find the number of bits required to represent c. */
    unsigned int bits = bitlen_seq_t(c);   /* bits >= 1 (c>0 for all n>=0) */

    /* Highest power of 2 <= c. */
    seq_t power = ((seq_t)1) << (bits - 1);

    /* If c is not a power of two, go to the next power of two. */
    if (power != c)
        power <<= 1;

    /* Return the difference. */
    return power - c;
}
