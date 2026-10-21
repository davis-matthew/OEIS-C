/*
 * A056486  a(n) = (9*2^n + (-2)^n)/4 for n>0.
 * https://oeis.org/A056486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*2^n + (-2)^n)/4. Equivalent closed forms: * a(n) =
 *            2^(n+1) if n is odd, * a(n) = 5 * 2^(n-1) if n is even. * This
 *            implementation uses bit‑shifts, valid for any seq_t (fixed or
 *            arbitrary precision)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1, as per OEIS offset */
    if ((n & 1) != 0) {               /* n odd   */
        return ((seq_t)1) << (n + 1); /* 2^(n+1) */
    } else {                           /* n even  */
        return ((seq_t)5) * (((seq_t)1) << (n - 1)); /* 5*2^(n-1) */
    }
}
