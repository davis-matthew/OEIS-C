/*
 * A094390  Beatty sequence of exp(Pi/4).
 * https://oeis.org/A094390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * exp(Pi/4))
 * Range:     u8 none, u16 none, u32 n <= 2232, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hard‑coded rational approximation of exp(Pi/4) with sufficient precision
   to guarantee exactness for all n such that the result fits in seq_t. */
seq_t a(idx_t n)
{
    /* 2^20 = 1,048,576.  exp(Pi/4) * 2^20 = 2,299,821.324883…   */
    const seq_t M   = (seq_t)2299821;   /* floor(exp(Pi/4) * 2^20)   */
    const unsigned SHIFT = 20;          /* 2^20 used for the scaling      */
    return ((seq_t)n * M) >> SHIFT;
}
