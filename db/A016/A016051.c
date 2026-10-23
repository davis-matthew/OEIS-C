/*
 * A016051  Numbers of the form 9*k+3 or 9*k+6.
 * https://oeis.org/A016051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 3*floor((n-1)/2) (n ≥ 1)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset not needed; return 0 for unspecified values */
    seq_t sn   = (seq_t)n;                 /* cast index to sequence type */
    seq_t half = (seq_t)((n - 1) / 2);     /* integer division, safe for unsigned n */
    return 3 * sn + 3 * half;              /* compute the closed‑form value */
}
