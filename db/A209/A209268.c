/*
 * A209268  Inverse permutation A054582.
 * https://oeis.org/A209268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T_p + k * p + k*(k-1)/2 * where * k = v₂(n) (the exponent
 *            of the largest power of 2 dividing n) * p = ( odd_part(n) + 1 )
 *            / 2 * T_p = p*(p+1)/2 * and odd_part(n) = n >> k. * This
 *            implements the permutation A209268 (inverse of A054582)
 * Range:     u8 n <= 44, u16 n <= 722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t k = 0;
    /* count trailing zeros to find exponent of 2 */
    while ((m & (idx_t)1) == 0) {
        m >>= 1;
        ++k;
    }
    idx_t y = m;            /* odd part of n */
    idx_t p = (y + 1) / 2;  /* index of odd number among odds */

    seq_t sp = (seq_t)p;
    seq_t sk = (seq_t)k;

    seq_t result = sp * (sp + 1) / 2;   /* triangular number T_p */
    result += sk * sp;                  /* k * p */

    if (k > 0) {
        result += sk * (sk - 1) / 2;    /* k*(k-1)/2 */
    }

    return result;
}
