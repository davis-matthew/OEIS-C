/*
 * A015281  Gaussian binomial coefficient [ n,3 ] for q = -12.
 * https://oeis.org/A015281
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..3} ((-12)^(n-i+1)-1)/((-12)^i-1) Computed
 *            by the recurrence for the q‑Gaussian binomial coefficient [n,3]
 *            with q = -12: G(n,0)=1, and G(n,k) = G(n-1,k) + q^(n-k) *
 *            G(n-1,k-1) (k=1,2,3)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* The function uses only seq_t arithmetic; it will work for
   int8_t, int16_t, int32_t, int64_t, __int128 and for arbitrary
   precision integer classes. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                 /* undefined for n<3, not used */

    const seq_t q = -12;
    seq_t g1 = 0, g2 = 0, g3 = 0;   /* G(n,1), G(n,2), G(n,3) at current n */

    /* current powers of q: pow[0]=q^(i-1), pow[1]=q^(i-2), pow[2]=q^(i-3) */
    seq_t pow[3] = { (seq_t)1, (seq_t)0, (seq_t)0 };  /* start with i=1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t qpow_i_minus1 = pow[0];
        seq_t qpow_i_minus2 = (i >= 2) ? pow[1] : (seq_t)0;
        seq_t qpow_i_minus3 = (i >= 3) ? pow[2] : (seq_t)0;

        /* recurrence updates using previous g1,g2,g3 */
        seq_t new_g1 = g1 + qpow_i_minus1;          /* + q^(i-1)*G(n-1,0)  */
        seq_t new_g2 = g2 + qpow_i_minus2 * g1;     /* + q^(i-2)*G(n-1,1)  */
        seq_t new_g3 = g3 + qpow_i_minus3 * g2;     /* + q^(i-3)*G(n-1,2)  */

        /* shift powers for next iteration: multiply the oldest by q */
        pow[2] = pow[1];
        pow[1] = pow[0];
        pow[0] = qpow_i_minus1 * q;                 /* q^i */

        g1 = new_g1;
        g2 = new_g2;
        g3 = new_g3;
    }
    return g3;
}
