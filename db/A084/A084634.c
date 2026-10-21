/*
 * A084634  Binomial transform of 1, 1, 1, 2, 2, 2, 2, 2, ...
 * https://oeis.org/A084634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - (n^2 + n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^(n+1) – computed by repeated left shift to avoid UB */
    idx_t shift   = n + 1;
    seq_t pow2    = (seq_t)1;
    for (idx_t i = 0; i < shift; ++i)
        pow2 <<= 1;                 /* wraps to 0 when the bits exceed the type */

    /* (n^2 + n + 2)/2  =  (n*(n+1))/2 + 1  */
    seq_t half = ((seq_t)n * ((seq_t)n + 1)) >> 1;
    seq_t sub  = half + 1;

    return pow2 - sub;
}
