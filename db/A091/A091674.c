/*
 * A091674  Numerator Q of probability P = Q(n)/365^(n-1) that two or more out
 *          of n people share the same birthday.
 * https://oeis.org/A091674
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 365^(n-1) - Π_{i=1}^{n-1} (365-i) (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   pow_365
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (the product is taken over 1 ≤ i < n; for n > 366 the product becomes 0) */

#include <stdbool.h>
#include "../utils/pow_365.h"

/* Return the n‑th term, where a(2) is the first term.            */
seq_t a(idx_t n)
{
    /* n < 2 is undefined, but we simply return 0 here. */
    if (n < 2)
        return 0;

    /* Pre–compute the cumulative products of (365‑i) once only.   */
    /* preprod[k] = product_{i=1}^{k} (365‑i), 0 ≤ k ≤ 365.       */
    static seq_t preprod[366];
    static bool init = false;
    if (!init)
    {
        preprod[0] = 1;
        for (idx_t i = 1; i <= 365; ++i)
            preprod[i] = preprod[i - 1] * (seq_t)(365 - i);
        init = true;
    }

    /* Compute the product term.  For n-1 > 365 it is zero.        */
    seq_t prod;
    if ((n - 1) <= 365)
        prod = preprod[n - 1];
    else
        prod = 0;

    /* Compute 365^(n-1). */
    seq_t pow_val = pow_365(n - 1);

    /* The numerator Q(n) = 365^(n-1) – product.                    */
    return pow_val - prod;
}
