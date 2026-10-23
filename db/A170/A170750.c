/*
 * A170750  Expansion of g.f.: (1+x)/(1-30*x).
 * https://oeis.org/A170750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 ; for n>0, a(n) = 31*30^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;          /* res = 1 for the exponentiation 30^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i) {
        res = res * (seq_t)30;    /* multiply by 30 progressively */
    }
    return res * (seq_t)31;       /* multiply by 31 to get the final term */
}
