/*
 * A052572  Expansion of e.g.f.: (1+2*x-2*x^2)/(1-x)^2.
 * https://oeis.org/A052572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*n! for n ≥ 1 (with a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values explicitly. */
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* Compute by recurrence:
       a(n) = a(n-1) / (n+2) * n * (n+3)
       The division is exact for all n ≥ 2. */
    seq_t res = 4;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t div   = (seq_t)(i + 2);          /* (i+2) fits in seq_t   */
        seq_t temp  = res / div;               /* exact division */
        res = temp * (seq_t)i * (seq_t)(i + 3);/* multiply by i and (i+3) */
    }
    return res;
}
