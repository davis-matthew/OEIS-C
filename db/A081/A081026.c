/*
 * A081026  Variation on Ulam numbers: a(1) = 1; a(2) = 2; for n>2, a(n) =
 *          smallest (n odd) or largest (n even) number > a(n-1) that is a
 *          unique sum of two distinct earlier terms.
 * https://oeis.org/A081026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for k>=1 a(2k) = 3*2^(k-1)-1 a(2k+1) = 3*2^(k-1)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* k = floor(n/2) */
    idx_t k = n >> 1;              /* k >= 1 for all n >= 2 */

    /* 1 << (k-1) works because k-1 <= maximum exponent that fits in seq_t */
    seq_t pow = ((seq_t)1) << (k - 1);
    seq_t val = (seq_t)3 * pow;

    if (n & 1) {                  /* odd (2k+1) */
        return val;
    } else {                      /* even (2k) */
        return val - (seq_t)1;
    }
}
