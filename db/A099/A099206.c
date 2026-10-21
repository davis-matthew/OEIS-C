/*
 * A099206  Expansion of
 *          x^2*(2*x^11+2*x^9+2*x^8+x^7+2*x^6+x^5+x^4+x^3-x^2-x-1) /
 *          (x^9+x^6+2*x^3-1).
 * https://oeis.org/A099206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A099206: Expansion of 
 *     x^2*(2*x^11+2*x^9+2*x^8+x^7+2*x^6+x^5+x^4+x^3-x^2-x-1) / (x^9+x^6+2*x^3-1)
 * 
 * The rational generating function yields the linear recurrence (for k ≥ 0)
 *     a(k) = a(k-9) + a(k-6) + 2*a(k-3) - c(k)
 * where the offset terms c(k) come from the numerator polynomial N(x) =
 *     2x^13 + 2x^11 + 2x^10 + x^9 + 2x^8 + x^7 + x^6 + x^5 - x^4 - x^3 - x^2
 * and are zero for k < 2 or k > 13.  The sequence is indexed from a(1) = 0
 * (as in the OEIS offset), so the array element a[0] is unused but set to 0 for
 * convenience in the recurrence.  All arithmetic fits in seq_t for the
 * supplied build; intermediate additions/subtractions are performed
 * carefully to avoid signed‑to‑unsigned conversion bugs.
 */

#include <stddef.h>
#include <stdlib.h>

static const int coeff_N[14] = {
    0,          /* k = 0  */
    0,          /* k = 1  */
   -1,          /* k = 2  */
   -1,          /* k = 3  */
   -1,          /* k = 4  */
    1,          /* k = 5  */
    1,          /* k = 6  */
    1,          /* k = 7  */
    2,          /* k = 8  */
    1,          /* k = 9  */
    2,          /* k = 10 */
    2,          /* k = 11 */
    0,          /* k = 12 */
    2           /* k = 13 */
    /* k ≥ 14  : 0 (implicit) */
};

seq_t a(idx_t n)
{
    /* Allocate array 0 … n */
    seq_t *a_val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_val) return (seq_t)0;   /* out‑of‑memory guard */

    a_val[0] = (seq_t)0;           /* unused but simplifies recurrence */

    /* Compute a(k) for k = 1 … n */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)0;

        if (k >= 9) term += a_val[k - 9];
        if (k >= 6) term += a_val[k - 6];
        if (k >= 3) term += a_val[k - 3] + a_val[k - 3];   /* 2*a(k-3) */

        int ck = (k <= 13) ? coeff_N[k] : 0;
        if (ck < 0)
            term += (seq_t)(-ck);
        else
            term -= (seq_t)ck;

        a_val[k] = term;
    }

    seq_t result = a_val[n];
    free(a_val);
    return result;
}
