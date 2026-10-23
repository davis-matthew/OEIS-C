/*
 * A025807  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A025807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 14*n + 123 + 17*(-1)^n + 28*[(n mod 5) in
 *            {2,4}] - 56*[(n mod 5)=3]) / 140)
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t to avoid overflow of the index type */
    seq_t sn     = (seq_t)n;
    seq_t n_sq   = sn * sn;
    seq_t term   = n_sq + (seq_t)14 * sn + (seq_t)123;

    /* add 17*(-1)^n :  +17 if n even, -17 if n odd */
    if ((n & 1) == 0)      /* n even */
        term += (seq_t)17;
    else                   /* n odd */
        term -= (seq_t)17;

    /* add 28 if n mod 5 is 2 or 4 */
    uint64_t r5 = n % 5;
    if (r5 == 2 || r5 == 4)
        term += (seq_t)28;

    /* subtract 56 if n mod 5 == 3 */
    if (r5 == 3)
        term -= (seq_t)56;

    /* divide by 140 – truncation works as floor for non‑negative values */
    return term / (seq_t)140;
}
