/*
 * A202094  Number of (n+2) X 4 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [C(floor((n+3)/2)+2,2) * C(floor((n+2)/2)+2,2)]^2 where
 *            C(k,2) = k*(k-1)/2. For n>=1 this yields the OEIS sequence
 *            A202094
 * Range:     u8 none, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* Outside OEIS offset, defined as 0 for safety */
        return 0;

    /* Compute floor((n+3)/2) and floor((n+2)/2) */
    idx_t alpha = (n + 3) / 2;
    idx_t beta  = (n + 2) / 2;

    /* C(alpha+2,2) = (alpha+2)*(alpha+1)/2 */
    seq_t p = (seq_t)(alpha + 2) * (seq_t)(alpha + 1) / 2;

    /* C(beta+2,2)  = (beta+2)*(beta+1)/2 */
    seq_t q = (seq_t)(beta + 2) * (seq_t)(beta + 1) / 2;

    seq_t r = p * q;     /* product of the two binomials */
    return r * r;        /* square gives the required term */
}
