/*
 * A133328  Values of n such that the sum of the 7-gonal number (5*n^2 -
 *          3*n)/2 and the following 7-gonal number is a 7-gonal number.
 * https://oeis.org/A133328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≥ 0, let P7(n) = (5*n^2 - 3*n)/2 be the n‑th 7‑gonal
 *            number. The n for which P7(n)+P7(n+1) is again a 7‑gonal number
 *            satisfy 200 n^2 + 80 n + 49 = k^2 for some integer k ≡ 7 (mod
 *            10). The sequence values a(n) are the indices n (starting with
 *            a(1)=0). Each parity (even/odd indices) satisfies...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   even_term, odd_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/even_term.h"
#include "../utils/odd_term.h"

/* External sequence function */
seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term in the list (0) */
    if (n % (idx_t)2 == (idx_t)1)   /* odd index */
        return odd_term((n + (idx_t)1) / (idx_t)2);
    else                            /* even index */
        return even_term(n / (idx_t)2);
}
