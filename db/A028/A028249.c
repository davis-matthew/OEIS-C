/*
 * A028249  Molien series for complete weight enumerator of self-dual code
 *          over GF(4) containing 1^n.
 * https://oeis.org/A028249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n³+12·n²+72·n+216 - 9·(n mod 2)·n - 24·((2·n²+n)
 *            mod 3)·n)/216)
 * Range:     u8 n <= 0, u16 n <= 36, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* n³ + 12·n² + 72·n + 216 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term1 = n3 + (seq_t)12 * n2 + (seq_t)72 * nn + (seq_t)216;

    /* subtract 9·(n mod 2)·n */
    seq_t term2_mul = 0;
    if ((nn & 1) != 0)          /* n is odd */
        term2_mul = (seq_t)9 * nn;

    /* subtract 24·((2·n² + n) mod 3)·n */
    int n_mod3 = (int)(nn % (idx_t)3);
    int mod_val = (2 * n_mod3 * n_mod3 + n_mod3) % 3;   /* (2·n² + n) mod 3 */
    seq_t term3_mul = (seq_t)24 * (seq_t)mod_val * nn;

    seq_t numer = term1 - term2_mul - term3_mul;
    return numer / (seq_t)216;   /* integer division floors automatically */
}
