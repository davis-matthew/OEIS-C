/*
 * A097950  G.f.: (1+x^5+x^10)/((1-x)*(1-x^3)).
 * https://oeis.org/A097950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (⌊n/3⌋+1) + (⌊(n-5)/3⌋+1 for n≥5) + (⌊(n-10)/3⌋+1 for
 *            n≥10)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: the coefficient of xⁿ in (1+x⁵+x¹⁰)/((1-x)(1-x³)) */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* first term, always present for n ≥ 0 */
    res += (seq_t)(n / 3) + 1;

    /* second term, present when n ≥ 5 */
    if (n >= 5) {
        res += (seq_t)((n - 5) / 3) + 1;
    }

    /* third term, present when n ≥ 10 */
    if (n >= 10) {
        res += (seq_t)((n - 10) / 3) + 1;
    }

    return res;
}
