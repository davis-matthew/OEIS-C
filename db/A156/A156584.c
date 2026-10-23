/*
 * A156584  Triangle T(n,k) = SF(n+1)/(SF(n-k+1)*SF(k+1)) where SF(n) is the
 *          superfactorial A000178(n), read by rows.
 * https://oeis.org/A156584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = SF(r+1)/(SF(r-k+1)*SF(k+1)) with
 *            SF(m)=∏_{i=1}^{m} i! . The index n is flattened row‑by‑row; r is
 *            the row and k the position within that row. The implementation
 *            avoids intermediate overflow by computing the product of
 *            integers raised to calculated exponents
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 38, u64 n <= 58, u128 n <= 109,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position k within row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Parts for exponent calculation. */
    idx_t a_part = r - k + 1;
    idx_t b_part = k + 1;
    idx_t lower = (a_part < b_part) ? a_part : b_part;
    idx_t upper = (a_part > b_part) ? a_part : b_part;

    seq_t result = 1;   /* identity element */

    /* Compute contribution of each integer t (1 ≤ t ≤ r+1). */
    for (idx_t t = 1; t <= r + 1; ++t)
    {
        int exp_t = 0;

        if (t <= upper)
            exp_t += (int)(r - upper + 1);
        else
            exp_t += (int)(r - t + 2);

        if (t <= lower)
            exp_t -= (int)(lower - t + 1);

        if (exp_t > 0)
            result = result * big_pow((seq_t)t, (seq_t)exp_t);
        else if (exp_t < 0)
            result = result / big_pow((seq_t)t, (seq_t)(-exp_t));
    }

    return result;
}
