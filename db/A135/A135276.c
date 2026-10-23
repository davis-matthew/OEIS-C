/*
 * A135276  a(0)=0, a(1)=1; for n>1, a(n) = a(n-1) + n^0 if n odd, a(n) =
 *          a(n-1) + n^1 if n is even.
 * https://oeis.org/A135276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)/2)^2 if n is odd, a(n) = (n/2 + 1)^2 - 1 if n is
 *            even (this expression works for all n ≥ 0)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;
    /* Use bitwise test for odd/even. */
    if ((nn & 1) == 0) {            /* even n */
        seq_t k = nn / 2 + 1;
        return k * k - 1;
    } else {                        /* odd n */
        seq_t k = (nn + 1) / 2;
        return k * k;
    }
}
