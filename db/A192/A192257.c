/*
 * A192257  1-sequence of reduction of (n^3) by x^2 -> x+1.
 * https://oeis.org/A192257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192257: 1-sequence of reduction of (n^3) by x^2 → x+1.
   The generating function is
        G(x) = x^2*(8-5*x+36*x^2-19*x^3+12*x^4-x^5) / ((1-x)*(1-x-x^2)^4).
   From G(x) we obtain the linear recurrence with constant coefficients
        a(n) = 8                      (n=2)
        a(n) = -5*a(n-1) + 6*a(n-2) + 6*a(n-3) -13*a(n-4) -3*a(n-5)
                +10*a(n-6) + 2*a(n-7) -3*a(n-8) - a(n-9)  (n≥10)
   with initial values a(0)=a(1)=0, a(2)=8, a(3)=35, a(4)=163, a(5)=538, a(6)=1618,
   a(7)=4362, a(8)=11018, a(9)=26327.  The code below implements this recurrence
   by direct iteration.  */

#include <stdlib.h>

static const int d_coeff[10] = { 1, -5,  6,  6, -13, -3, 10,  2, -3, -1 };
static const int n_coeff[10] = { 0,  0,  8, -5,  36, -19, 12, -1,  0,  0 };

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* a(0) and a(1) are 0 */

    /* Allocate space for the sequence up to n.  */
    seq_t *s = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!s) return 0;                  /* allocation failure -> 0 */

    s[0] = 0;
    s[1] = 0;
    s[2] = 8;                          /* first non‑zero term */

    /* Compute subsequent terms using the recurrence derived from the
       generating function.  The recurrence involves 9 previous terms.  */
    for (idx_t i = 3; i <= n; ++i)
    {
        __int128_t tmp = 0;
        /* N(x) contributes only for i < 10 */
        if (i < 10)
            tmp += (__int128_t)n_coeff[i];

        /* Subtract d_k * a(n-k) for k = 1 … 9, but i must be ≥ k. */
        for (int k = 1; k <= 9; ++k)
        {
            if (i >= (idx_t)k)
                tmp -= (__int128_t)d_coeff[k] * (__int128_t)s[i - k];
        }
        s[i] = (seq_t)tmp;              /* tmp is guaranteed non‑negative */
    }

    seq_t result = s[n];
    free(s);
    return result;
}
