/*
 * A052760  Expansion of e.g.f.: x^2*(exp(x)-1)^2.
 * https://oeis.org/A052760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 (n < 4); a(n) = n * (n-1) * (2^n - 8) / 4 (n ≥ 4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero for the initial offset */
    if (n < 4)
        return (seq_t)0;

    /* Compute 2^n as a seq_t value by repeated doubling */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* 2^n - 8  (this is always a multiple of 8 for n ≥ 3) */
    seq_t term = pow2 - (seq_t)8;

    /* Divide the term by 4 first – safe because term is divisible by 8 */
    seq_t term_div4 = term / (seq_t)4;

    /* Remaining factors n and (n-1) multiply safely for values that fit */
    seq_t prod = (seq_t)n * (seq_t)(n - 1);

    return prod * term_div4;
}
