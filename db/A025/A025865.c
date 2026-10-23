/*
 * A025865  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A025865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 27*n + 432)/432 + (n+8)(-1)^n/48 - (n+15)(n
 *            mod 3)/54)
 * Range:     u8 n <= 0, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Common denominator 432 */
    seq_t numerator = nn * nn                 /* n^2          */
                    + (seq_t)27 * nn          /* +27n         */
                    + (seq_t)432;             /* +432         */

    /* (n+8)(-1)^n/48  →  (n+8)*(-1)^n * 9 / 432  */
    seq_t partB = (seq_t)9 * (nn + (seq_t)8);
    if ((n & (idx_t)1) == 0)          /* n even → add */
        numerator += partB;
    else                              /* n odd  → subtract */
        numerator -= partB;          /* safe: numerator ≥ partB */

    /* -(n+15)(n mod 3)/54  →  -(n+15)(n mod 3) * 8 / 432  */
    idx_t r = n % (idx_t)3;
    if (r != 0)
    {
        seq_t partC = (seq_t)8 * (nn + (seq_t)15) * (seq_t)r;
        numerator -= partC;          /* safe: numerator ≥ partC + partB(for odd) */
    }

    return numerator / (seq_t)432;    /* integer division = floor for n≥0 */
}
