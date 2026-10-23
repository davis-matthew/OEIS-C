/*
 * A143730  a(n) = A141616(n) - A100181(n).
 * https://oeis.org/A143730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 + 2*j, where j is the smallest non‑negative integer
 *            such that sum_{i=0}^{j} (floor(i/2)+1) >= n. This implements the
 *            difference A141616(n) – A100181(n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* not used for n>=1, just a safety guard */
        return 0;

    idx_t j   = 0;
    idx_t cum = 0;

    while (cum < n)
    {
        idx_t g = (j >> 1) + 1;   /* floor(j/2)+1  - size of the j‑th odd number block */
        cum += g;
        if (cum >= n)
            break;
        ++j;
    }

    return (seq_t)5 + (seq_t)2 * (seq_t)j;
}
