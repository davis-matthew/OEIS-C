/*
 * A086224  a(n) = 7*2^n - 1.
 * https://oeis.org/A086224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n - 1 (OEIS A086224)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n.  For integer built‑ins this is a shift;  for an
       arbitrary‑precision seq_t the operator<< is assumed to be defined
       as the usual big‑integer shift.  The intermediate multiplication
       and subtraction are performed in seq_t as well.  This routine
       returns the exact value whenever it fits in seq_t. */
    seq_t pow2 = ((seq_t)1 << n);      /* 2^n */
    seq_t res   = pow2 * (seq_t)7;     /* 7*2^n */
    res -= (seq_t)1;                   /* 7*2^n - 1 */
    return res;
}
