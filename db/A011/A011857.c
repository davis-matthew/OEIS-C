/*
 * A011857  Triangle of numbers [ C(n,k)/k ], k=1..n-1.
 * https://oeis.org/A011857
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = ⌊C(r,k)/k⌋ where r,k satisfy the flattened * index
 *            mapping of the triangle with rows 2,3,4,… and entries * C(r,k)/k
 *            for k = 1 … r−1
 * Range:     u8 n <= 71, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /*--- Map the flattened index to row r and column k. --------*/
    idx_t remaining = n - 2;          /* 0‑based counter starting at a(2) */
    idx_t r = 2;

    /* Consume complete rows until the row containing the term is found. */
    while (remaining >= (r - 1)) {
        remaining -= (r - 1);
        ++r;
    }

    idx_t k = remaining + 1;          /* 1 ≤ k ≤ r‑1 */

    /*--- Compute C(r,k)/k. ------------------------------------*/
    /* If seq_t is a built‑in integer type, use 128‑bit arithmetic
     * to avoid overflow during the binomial calculation. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 prod = 1;
        idx_t kk = k;
        if (kk > r - kk)          /* use symmetry */
            kk = r - kk;

        for (idx_t i = 1; i <= kk; ++i)
            prod = prod * (unsigned __int128)(r - kk + i) / (unsigned __int128)i;

        unsigned __int128 result = prod / (unsigned __int128)k;
        return (seq_t)result;
    }

    /* For arbitrary‑precision seq_t, use its native operators. */
    seq_t prod = 1;
    idx_t kk = k;
    if (kk > r - kk)
        kk = r - kk;

    for (idx_t i = 1; i <= kk; ++i)
        prod = prod * (seq_t)(r - kk + i) / (seq_t)i;

    return prod / (seq_t)k;
}
