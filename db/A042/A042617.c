/*
 * A042617  Denominators of continued fraction convergents to sqrt(837).
 * https://oeis.org/A042617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents of sqrt(837).
   Recurrence: a(0)=1, a(1)=1,
   a(n) = c[n]*a(n-1)+a(n-2) for n>=2,
   where the partial quotient cycle c is [1,13,2,13,1,56] repeating. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    static const seq_t coeffs[6] = {(seq_t)1,(seq_t)13,(seq_t)2,(seq_t)13,(seq_t)1,(seq_t)56};
    seq_t prev2 = (seq_t)0; /* a(-1) */
    seq_t prev1 = (seq_t)1; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = coeffs[(i-1) % 6];
        seq_t curr = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
