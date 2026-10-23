/*
 * A112423  Number of 6-element subsets of {1,2,3,...,n} which have a sum-set
 *          with 14 elements.
 * https://oeis.org/A112423
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula derived from the given generating function:
   6*x^8/((1-x)^2*(1-x^7)) + 12*x^9/((1-x)^2*(1-x^8))
   a(n) = 3*(m+1)*(2*(n-7)-7*m) + (n>=9?12*(p+1)*(n-8-4*p):0)
   where m = ⟂(n-8)/7⟂ and p = ⟂(n-9)/8⟂
   (⟂ ⟂ denotes integer division for non‑negative numerators). */
seq_t a(idx_t n)
{
    if (n < 8) return 0;   /* sequence is defined for n >= 8 */

    idx_t m = (n - 8) / 7;          /* floor((n-8)/7)   */
    seq_t mS = (seq_t)m;
    seq_t term1 = 3 * (mS + 1) *
                  ((seq_t)(2 * ((seq_t)n - 7)) - 7 * mS);

    seq_t result = term1;

    if (n >= 9) {
        idx_t p = (n - 9) / 8;      /* floor((n-9)/8)   */
        seq_t pS = (seq_t)p;
        seq_t term2 = 12 * (pS + 1) *
                      ((seq_t)n - 8 - 4 * pS);
        result += term2;
    }

    return result;
}
