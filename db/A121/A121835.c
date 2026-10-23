/*
 * A121835  Hankel transform of A014307.
 * https://oeis.org/A121835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} ((2k-1)!! * k!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t df      = 1;   /* (2k-1)!! for current k */
    seq_t fac     = 1;   /* k! for current k */

    /* For n = 0 or n = 1, result stays 1 (empty product). */
    for (idx_t k = 2; k <= n; ++k) {
        df  *= ((seq_t)2 * k - 1); /* update (2k-1)!! */
        fac *= (seq_t)k;           /* update k!       */
        result *= df;
        result *= fac;
    }
    return result;
}
