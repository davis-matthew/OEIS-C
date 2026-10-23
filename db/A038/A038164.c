/*
 * A038164  Expansion of 1/((1-x)*(1-x^2))^4.
 * https://oeis.org/A038164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*k) = (4*k^2 + 24*k + 21)*binomial(k + 5, 5)/21 (k ≥ 0)
 *            a(2*k+1) = 4*binomial(k + 6, 6)*(7 + 2*k)/7 (k ≥ 0) These
 *            formulas are integer‑valued for all k
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t even_term(idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t bin   = binomial(k + 5, 5);
    seq_t factor = ( (seq_t)4 * k_seq * k_seq + (seq_t)24 * k_seq + (seq_t)21 );
    return (factor * bin) / 21;
}

static seq_t odd_term(idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t bin   = binomial(k + 6, 6);
    seq_t temp  = ( (seq_t)7 + (seq_t)2 * k_seq );
    return ( (seq_t)4 * bin * temp ) / 7;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return even_term(n / 2);
    else
        return odd_term((n - 1) / 2);
}
