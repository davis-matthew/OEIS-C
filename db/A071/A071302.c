/*
 * A071302  a(n) = (1/2) * (number of n X n 0..2 matrices M with MM' mod 3 =
 *          I, where M' is the transpose of M and I is the n X n identity
 *          matrix).
 * https://oeis.org/A071302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n is odd: a(n) = 3^k * ∏_{i=0..k-1} (3^{2k} - 3^{2i}),
 *            k=(n-1)/2 if n is even: a(n) = (3^k + (-1)^{k+1}) * ∏_{i=1..k-1}
 *            (3^{2k} - 3^{2i}), k=n/2 This directly implements the
 *            closed‑form from the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but just in case */
    idx_t k;
    int even = (n & 1UL) == 0;
    if (even) k = n >> 1;
    else      k = (n - 1) >> 1;

    /* precompute powers of 3 up to 3^{2k} */
    size_t len = (size_t)(2 * k + 1);
    seq_t *pow3 = (seq_t *)malloc(len * sizeof(seq_t));
    if (!pow3) return 0;  /* memory error */
    pow3[0] = (seq_t)1;
    for (idx_t t = 1; t <= 2 * k; ++t)
        pow3[t] = pow3[t - 1] * (seq_t)3;

    seq_t base = pow3[2 * k];          /* 3^{2k} */
    seq_t result = (seq_t)1;

    if (even) {
        /* term = 3^k + (-1)^{k+1} */
        seq_t term = pow3[k];
        if ((k & 1UL) == 0) term -= (seq_t)1;  /* k even => -1 */
        else                term += (seq_t)1;  /* k odd  => +1 */
        result = term;

        /* product over i = 1 .. k-1 */
        for (idx_t i = 1; i < k; ++i) {
            seq_t sub = pow3[2 * i];
            result *= (base - sub);
        }
    } else { /* odd */
        result = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) {
            seq_t sub = pow3[2 * i];
            result *= (base - sub);
        }
        result *= pow3[k];
    }

    free(pow3);
    return result;
}
